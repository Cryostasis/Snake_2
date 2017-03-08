#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996)
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <shellapi.h>
#include <iostream>

#include "winfuncs.h"
#include "control_panel.h"
#include <wx\wx.h>
#include <cstdlib>

using namespace std;

void init()
{
	srand((unsigned int)time(NULL));

	ControlForm* app = new ControlForm();
	wxApp::SetInstance(app);
	wxEntry(0, NULL);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//setlocale(LC_ALL, "Russian");
	//create_console();

	init();

	return 0;
}
