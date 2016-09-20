#include "Game.h"

HINSTANCE _hinst;
HWND _hwnd;
Game *_game;

//-----------------------------------------------------------------------------
// Window event callback function
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	return _game->messageHandler(hwnd, msg, w, l);
}

//-----------------------------------------------------------------------------
// Create and register a Window class
bool CreateMainWindow(HINSTANCE hinst, int cmd)
{
	WNDCLASSEX wcx;
	wcx.cbClsExtra = 0;
	wcx.cbSize = sizeof(wcx);
	wcx.cbWndExtra = 0;
	wcx.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcx.hCursor = LoadCursor(0, IDC_ARROW);
	wcx.hIcon = 0;
	wcx.hIconSm = 0;
	wcx.hInstance = hinst;
	wcx.lpfnWndProc = WinProc;
	wcx.lpszClassName = APP_NAME;
	wcx.lpszMenuName = 0;
	wcx.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcx))
	{
		throw (Error(ERRORNS::FATAL_ERROR, "Loi xuat hien tai registerclassex"));
		return false;
	}

	// Create Window
	_hwnd = CreateWindow(
		APP_NAME, // Name of the window class
		WINDOW_NAME, // Title bar text
		WS_OVERLAPPEDWINDOW, // style normal window
		0, // Horizontal position of window
		0, // vertical position of window
		WINDOW_WIDTH, 
		WINDOW_HEIGHT,
		0, // No parent window
		0, // No menu
		hinst, // Handle to application instance
		0 // No window parameters
		);

	if (!_hwnd)
	{
		throw(Error(ERRORNS::FATAL_ERROR, "Error at Createwindow"));
		return false;
	}

	ShowWindow(_hwnd, cmd); // Show the window
	UpdateWindow(_hwnd); // Send the window a message to paint itself
	return true;
}

//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE p, HINSTANCE n, LPSTR lp, int cmd)
{
	MSG msg;
	_game = new Game;

	try
	{
		if (!CreateMainWindow(_hinst, 1))
			return -1;

		_game->initialize(_hwnd, WINDOW_WIDTH, WINDOW_HEIGHT, FULLSCREEN);
		
		DWORD frame_start = GetTickCount();
		DWORD tick_per_frame = 1000 / FRAME_RATE;

		while (1)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					break;
				}

				// Decode and pass messages on to WinProc
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				DWORD now = GetTickCount();
				if (now - frame_start >= tick_per_frame)
				{
					frame_start = now;
					_game->run(_hwnd);
				}
			}
		}
	}
	catch (const Error& e)
	{
		MessageBox(0, e.GetMsg(), "Error", MB_OK);
	}
	catch (const std::exception& e)
	{
		MessageBox(0, e.what(), "Error", MB_OK);
	}

	return 1;
}
