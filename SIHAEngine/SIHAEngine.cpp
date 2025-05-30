#include <iostream>
#include <Windows.h>
#include <windowsx.h>

// the WindowProc function prototype
// WindowProc 전방 선언
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
// 모든 윈도우 프로그램의 진입점
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hprevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	/*
	1. Register the window class
	2. Create the window
	3. Show the window
	*/

	// the handle for the window, filled by a function
	// 윈도우의 핸들, 함수에 의해 채워짐
	HWND hwnd;
	//this struct holds information for the window class
	//해당 구조체는 윈도우 클래스에 대한 정보를 보유합니다.
	WNDCLASSEX wc;

	//clear out the window class for use
	//사용을 위해 윈도우 클래스를 초기화 합니다.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//fill in the struct with the needed information
	//구조체에 필요한 정보를 채웁니다.

	// size of the window class
	wc.cbSize = sizeof(WNDCLASSEX);
	// use default class styles
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// WindowProc 함수를 lpfnWndProc에 저장합니다.
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	// IDCICON_SMALL,	//we're using a small icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// background color for the window
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	// class name in the window class
	wc.lpszClassName = L"WindowClass";

	//register the window class
	RegisterClassEx(&wc);

	RECT wr = { 0,0,500,400 }; // set the size, but not the position

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE); // adjust the size

	//Create the window and use the result as the handle
	hwnd = CreateWindowEx(NULL, 
						L"WindowClass",						//name of the window class
						L"Our First Windowed Program",	//title of the window
						WS_OVERLAPPEDWINDOW,			//window style
						300,							//x-position of the window
						300,							//y-position of the window
						wr.right - wr.left,				//width of the window
						wr.bottom - wr.top,				//height of the window
						NULL,							//we have no parent window, NULL
						NULL,							//we aren't using menus, NULL
						hInstance,						//application handle
						NULL);							//used with multiple windows, NULL

	//display the window on the screen
	//화면에 윈도우를 표사한다.
	ShowWindow(hwnd, nShowCmd);

	//enter the main loop;

	//this struct holds windows event messages
	//이 구조체는 윈도우 이벤트 메시지를 보유합니다.
	MSG msg = { 0 };

	//wait for the next message in the queue, store the result in 'msg'
	//큐에서 다음 메시지를 기다리고 , 결과를 'msg'에 저장합니다.
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//translate keystroke messages into the right format
			//변환 키 입력 메시지를 올바른 형식으로 변환합니다.
			TranslateMessage(&msg);
		
			//send the message to the windowProc function
			//windowProc에 메시지를 보냅니다.
			DispatchMessage(&msg);

			// if we see a quit message, exit the program
			if (msg.message == WM_QUIT)
				break;
		}
		else 
		{
			//run the game code
		}
	}

	//return this part of the WM_QUIT message it  Windows
	return msg.wParam;
}

// this is the main message to the WindowProc function
// 이것은 WindowProc 함수에 대한 정의부입니다.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//sort through and find what cide to run for the massage
	//메시지에 대해 어떤 코드를 실행할지 정렬합니다.
	switch (uMsg)
	{
		//this message is read when the window is closed
		//이 메시지는 창을 닫을때 읽어오는 메시지입니다.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}
	//Handle any messages the switch statement didn't
	//핸들이 스위치 문에서 처리되지 않은 경우 리턴합니다.
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}