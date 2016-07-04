/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	main.cpp
Description	:	main implementation file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // Include all the windows headers.
#include <windowsx.h> // Include useful macros.
#include <cmath>
#include <crtdbg.h>
#include <time.h>
#define WINDOW_CLASS_NAME L"WINCLASS1"
#include <time.h>
#include "ThreadPool.h"
#include "WorkQueue.h"
#include "Task.h"

ThreadPool _threadPool;

/***********************
*WindowProc: handle button presses
* @author: Joshua Tanner
* @parameter: HInstance, HWindow, wParam & lParam
* @return: LReselt
********************/
LRESULT CALLBACK
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	// This is the main message handler of the system.
	PAINTSTRUCT ps; // Used in WM_PAINT.
	HDC hdc; // Handle to a device context.
	// What is the message?
	switch (_uiMsg)
	{
	case WM_CREATE:
	{
		// Do initialization stuff here.
		_threadPool.Initialize(_hWnd, 800, 800);	
		// Return Success.
		return (0);
	}
		break;
	case WM_LBUTTONDOWN:
	{
		//Change the selected thread
		if (_threadPool.m_iSelectedThread == _threadPool.m_iNumberOfThreads)
		{
			_threadPool.m_iSelectedThread = 0;
		}
		else
		{
			_threadPool.m_iSelectedThread++;
		}

		break;
		return(0);
	}
	case WM_PAINT:
	{
		// Simply validate the window.
		hdc = BeginPaint(_hWnd, &ps);			
	
		EndPaint(_hWnd, &ps);
		// Return Success.
		return (0);
	}
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return TRUE;
		// Return success.
		return (0);
	}
		break;
	default:break;
	} // End switch.
	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

/***********************
*CreateAndRegisterWindow: set up the window
* @author: Joshua Tanner
* @parameter: HInstance, window width & height and title of window
* @return: Handle to window
********************/
HWND CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, const wchar_t* _pcTitle)
{
	//Creating the window
	HBRUSH WhiteScreen = CreateSolidBrush(RGB(0, 0, 0));
	WNDCLASSEX winclass;
	winclass.hInstance = _hInstance;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_ERROR);
	winclass.hCursor = LoadCursor(NULL, IDC_HAND);
	winclass.hbrBackground = static_cast<HBRUSH> (GetStockObject(NULL_BRUSH));
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_ERROR);


	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	HWND hwnd;
	hwnd = CreateWindowEx(NULL,
		WINDOW_CLASS_NAME,
		_pcTitle,
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		_iWidth, _iHeight,
		NULL,
		NULL,
		_hInstance,
		NULL);

	if (!hwnd)
	{
		// Failed to create.
		return (0);
	}

	return (hwnd);
}


/***********************
* WinMain: run the program
* @author: Joshua Tanner
* @parameter: HInstance, handle to previous instance, _lpCmdline, _iCmdshow
* @return: int
********************/
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	
	HWND hwnd; // Generic window handle.
	MSG msg; // Generic message.
	ZeroMemory(&msg, sizeof(MSG));
	// First fill in the window class structure.
	
	 hwnd = CreateAndRegisterWindow(_hInstance, 800, 800, L"Thread Pool Mandelbrot Set Generation");
	 //For degugging 
	 _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int Threads = _threadPool.m_iNumberOfThreads;
	int Width = 800 / Threads;
	int Height = Width;

	//Split screen space into segments
	for (int i = 0; i < Threads; i++)
	{
		for (int j = 0; j < Threads; j++)
		{
			//Submit threads
			_threadPool.Submit(MandelbrotSegment(Width*j, Width*j + Width, Height*i, Height*i + Height));
		}
	}

	//Wait for calculation for finish before otuputting 
	while (_threadPool.m_bFinished != false)	{}

	//Wait for threads to join
	for (unsigned int i = 0; i < _threadPool.m_iNumberOfThreads; i++)
	{
		_threadPool.m_workerThreads[i].join();
	}
	_threadPool.SetEndTime();//Set the end time

	// Enter main event loop.
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			_threadPool.Draw();
		}
	}

	return (static_cast<int>(msg.wParam));
}
