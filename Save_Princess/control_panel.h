#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996)
#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/filepicker.h>

#include <queue>

#define FIELD_SIZE 30
enum direction {DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT};
typedef unsigned int uint;

class Snake
{
public:
	direction dir;
	std::deque<std::pair<int, int>> units;
};

class MainFrame : public wxFrame
{
private:
	std::pair<int, int> _food;
	Snake _snake;
protected:
	wxPanel* DrawPanel;
	wxStaticText* ScoreLabel;
	wxButton* StartButton;

	// Virtual event handlers, overide them in your derived class
	void StartButtonOnButtonClick(wxCommandEvent& event);
	void MainFrameOnKeyDown(wxKeyEvent& event);

	void new_food();
	void label_update();
public:
	wxTimer* timer;

	MainFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(620, 670), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MainFrame();

	void timerEvent(wxTimerEvent& event);

	void paintEvent(wxPaintEvent & evt);
	void paintNow();
	void render(wxDC& dc);
};

class ControlForm : public wxApp
{
public:
	MainFrame* form;
	ControlForm() {};
private:
	bool OnInit()
	{
		form = new MainFrame(NULL, -1, "Conrol panel");
		form->Show();
		return true;
	}
};

