#include "control_panel.h"

#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

bool dir_changed;
direction new_dir;
int score;
int timer_interval;

MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(620, 680);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWFRAME));

	wxBoxSizer* MainSizer;
	MainSizer = new wxBoxSizer(wxVERTICAL);

	DrawPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600, 600), wxTAB_TRAVERSAL);
	DrawPanel->SetBackgroundColour(wxColour(255, 255, 255));

	MainSizer->Add(DrawPanel, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	ScoreLabel = new wxStaticText(this, wxID_ANY, wxT("Score: 0"), wxDefaultPosition, wxDefaultSize, 0);
	ScoreLabel->Wrap(-1);
	ScoreLabel->SetFont(wxFont(12, 74, 90, 92, false, wxEmptyString));

	bSizer2->Add(ScoreLabel, 0, wxALL, 5);


	bSizer2->Add(0, 0, 1, wxEXPAND, 5);

	StartButton = new wxButton(this, wxID_ANY, wxT("Start!"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(StartButton, 0, wxALL, 5);


	MainSizer->Add(bSizer2, 1, wxEXPAND, 5);


	this->SetSizer(MainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	StartButton->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainFrame::MainFrameOnKeyDown), NULL, this);
	//this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainFrame::MainFrameOnKeyDown));
	//DrawPanel->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainFrame::MainFrameOnKeyDown), NULL, this);
	
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(MainFrame::timerEvent), NULL, this);
	DrawPanel->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::paintEvent), NULL, this);
	StartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::StartButtonOnButtonClick), NULL, this);

	timer = nullptr;
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	StartButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::StartButtonOnButtonClick), NULL, this);

}

pair<int, int> dir_by_dir(const direction dir)
{
	switch (dir)
	{
	case DIR_UP:	return{ 0, -1 };
	case DIR_DOWN:	return{ 0, 1 };
	case DIR_LEFT:	return{ -1, 0 };
	case DIR_RIGHT: return{ 1, 0 };
	}
}

pair<int, int> operator+(pair<int, int> left, pair<int, int> right)
{
	return {left.first + right.first, left.second + right.second};
}

void MainFrame::timerEvent(wxTimerEvent& event)
{
	_snake.dir = new_dir;
	pair<int, int> new_pos = _snake.units.back() + dir_by_dir(_snake.dir);

	bool flag = (new_pos.first >= FIELD_SIZE || new_pos.first < 0 || new_pos.second >= FIELD_SIZE || new_pos.second < 0);

	deque<pair<int, int>>::iterator it = _snake.units.begin();
	while (it != _snake.units.end())
	{
		flag = flag || (new_pos == *it);
		it++;
	}
	if (!flag)
	{
		if (new_pos == _food)
			new_food();
		else
			_snake.units.pop_front();
		_snake.units.push_back(new_pos);
	}
	timer->StartOnce(timer_interval);
	paintNow();
}


void MainFrame::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(DrawPanel);
	render(dc);
}

void MainFrame::paintNow()
{
	wxClientDC dc(DrawPanel);
	render(dc);
}

void MainFrame::render(wxDC&  dc)
{
	int SZ = DrawPanel->GetSize().GetHeight() / FIELD_SIZE;
	deque<pair<int, int>>::iterator it = _snake.units.begin();

	dc.Clear();
	dc.SetBrush(*wxBLACK_BRUSH);
	dc.SetPen(wxPen(wxColor(0, 255, 0), 2));
	while (it != _snake.units.end())
	{
		dc.DrawRectangle(SZ * it->first, SZ * it->second, SZ, SZ);
		it++;
	}

	dc.SetBrush(*wxRED_BRUSH);
	dc.SetPen(wxPen(wxColor(255, 0, 0), 2));
	dc.DrawCircle(SZ * _food.first + SZ / 2 - 1, SZ * _food.second + SZ / 2 - 1, SZ * 0.5 - 2);
}

void MainFrame::StartButtonOnButtonClick(wxCommandEvent & event)
{
	new_dir = DIR_RIGHT;
	deque<pair<int, int>> empty;
	swap(_snake.units, empty);
	_snake.units.push_front({ FIELD_SIZE / 2    , FIELD_SIZE / 2 });
	_snake.units.push_front({ FIELD_SIZE / 2 - 1, FIELD_SIZE / 2 });
	_snake.units.push_front({ FIELD_SIZE / 2 - 2, FIELD_SIZE / 2 });
	_snake.dir = new_dir;

	new_food();
	score = 0;
	label_update();

	timer_interval = 100;

	if (!timer)
		timer = new wxTimer(this);
	timer->StartOnce(timer_interval);
}

void MainFrame::MainFrameOnKeyDown(wxKeyEvent & event_)
{
	int i = event_.GetKeyCode();	

	switch (event_.GetKeyCode())
	{
	case WXK_LEFT:	new_dir = (_snake.dir == DIR_RIGHT	? new_dir : DIR_LEFT);  return;
	case WXK_UP:	new_dir = (_snake.dir == DIR_DOWN	? new_dir : DIR_UP);	  return;
	case WXK_RIGHT: new_dir = (_snake.dir == DIR_LEFT	? new_dir : DIR_RIGHT); return;
	case WXK_DOWN:	new_dir = (_snake.dir == DIR_UP		? new_dir : DIR_DOWN);  return;
	}
}

void MainFrame::new_food()
{
	score++;
	timer_interval--;
	bool flag = true;
	deque<pair<int, int>>::iterator it = _snake.units.begin();
	while (flag)
	{
		flag = false;
		_food = { rand() % FIELD_SIZE, rand() % FIELD_SIZE };
		while (it != _snake.units.end())
		{
			flag = flag || (_snake.units.back() == *it);
			it++;
		}
	}
	label_update();
}

void MainFrame::label_update()
{
	char * buf = new char[20];
	itoa(score, buf, 10);
	ScoreLabel->SetLabel("Score: " + string(buf));
	delete buf;
}
