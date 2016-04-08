/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-11-15
Project: Game Engine for Game programming for windows II

desc:
Winmain for game engine
Note:
Based on the information given in "Advanced 2D game deveolement"
Greatly simplified and personalized to be practical in this course.
************************************************************************/

#include "GameEngine.h"

// Macro used for key state
#define KEY_DOWN(vk) ((GetAsyncKeyState(vk) & 0x8000)?1:0)

HINSTANCE hInstance;
HWND hWnd;
int nCmdShow;

GameEngine::Engine *engine;
bool gameover;

LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		 case WM_QUIT:
		 case WM_CLOSE:
		 case WM_DESTROY:
			 gameover = true;
			 break;
	   }
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MSG msg;
	srand((unsigned int)time(NULL));
	hInstance = hInstance;
	nCmdShow = nCmdShow;
	DWORD dwStyle, dwExStyle;
	RECT windowRect;

	engine = new GameEngine::Engine();
	
	// Set params for game
	if (!game_preload()) {
		MessageBox(hWnd, "Error in game preload!", "Error", MB_OK);
		return 0; 
	}
	
	// Config window
	char title[255];
	sprintf(title, "%s", engine->getAppTitle().c_str());
	windowRect.left = (long)0;
	windowRect.right = (long)engine->getScreenWidth();
	windowRect.top = (long)0;
	windowRect.bottom = (long)engine->getScreenHeight();


   // Create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WinProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = NULL;
	wc.hCursor	   = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = title;
	wc.hIconSm	   = NULL;
	RegisterClassEx(&wc);

	// Set screen in windowed or fullscreen mode?
	if (engine->getFullscreen()) 
	{
	   DEVMODE dm;
	   memset(&dm, 0, sizeof(dm));
	   dm.dmSize = sizeof(dm);
	   dm.dmPelsWidth = engine->getScreenWidth();
	   dm.dmPelsHeight = engine->getScreenHeight();
	   dm.dmBitsPerPel = engine->getColorDepth();
	   dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	   if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
		  MessageBox(NULL, "Display mode failed", NULL, MB_OK);
		  engine->setFullscreen(false);
	   }

		dwStyle = WS_POPUP;
		dwExStyle = WS_EX_APPWINDOW;
		ShowCursor(FALSE);
	}
	else {
		dwStyle = WS_OVERLAPPEDWINDOW;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	// Adjust window to correct size
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	// Create the program window
	hWnd = CreateWindowEx( 0,
	   title,											//window class
	   title,											//title bar
	   dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,	  
	   0, 0,											//x,y coordinate
	   windowRect.right - windowRect.left,				//width of the window
	   windowRect.bottom - windowRect.top,				//height of the window
	   0,												//parent window
	   0,												//menu
	   hInstance,										//application instance
	   0);												//window parameters

	// Any errors?
	if (!hWnd) 	{
		MessageBox(hWnd, "Error creating program window!", "Error", MB_OK);
		return 0; 
	}

	// Display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Initialize game engine
	engine->setWindowHandle(hWnd);
	if (!engine->Init(engine->getScreenWidth(), engine->getScreenHeight(), engine->getColorDepth(), engine->getFullscreen())) 	{
		MessageBox(hWnd, "Error initializing the engine", "Error", MB_OK);
		return 0;
	}

	// Main loop
	gameover = false;
	while (!gameover)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		engine->Update();
	}

   if (engine->getFullscreen()) {
	  ShowCursor(TRUE);
   }

	delete engine;

   return 0;
}
