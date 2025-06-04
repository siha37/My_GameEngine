#include <Windows.h>
#include <windowsx.h>
#include <d3d9.h>


#define SCREEN_WIDTH 800 // width of the screen
#define SCREEN_HEIGHT 600 // height of the screen

//include the Direct3D Library file
#pragma comment(lib, "d3d9.lib")

//global declerations
LPDIRECT3D9 d3d = NULL; // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev = NULL; // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL; // pointer to the vertex buffer

//function prototypes
void initD3D(HWND hwnd); // sets up and initializes Direct3D
void render_frame(void); // renders a single frame
void cleanD3D(void); // closes Direct3D and releases memory
void init_graphics(void); // sets up the vertex buffer and the custom vertex format

struct CUSTOMVERTEX // create a struct for our custom vertex type
{
	FLOAT x, y, z; // the transformed position for the vertex
	DWORD color; // the vertex color
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE) // custom flexible vertex format

typedef struct _D3DMATRIX
{
	union {
		struct {
			float		_11, _12, _13, _14;
			float		_21, _22, _23, _24;
			float		_31, _32, _33, _34;
			float		_41, _42, _43, _44;
		};
		float m[4][4];
	}
};

// the WindowProc function prototype
// WindowProc ���� ����
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);





// the entry point for any Windows program
// ��� ������ ���α׷��� ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	/*
	1. Register the window class
	2. Create the window
	3. Show the window
	*/

	// the handle for the window, filled by a function
	// �������� �ڵ�, �Լ��� ���� ä����
	HWND hwnd;
	//this struct holds information for the window class
	//�ش� ����ü�� ������ Ŭ������ ���� ������ �����մϴ�.
	WNDCLASSEX wc;

	//clear out the window class for use
	//����� ���� ������ Ŭ������ �ʱ�ȭ �մϴ�.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//fill in the struct with the needed information
	//����ü�� �ʿ��� ������ ä��ϴ�.

	// size of the window class
	wc.cbSize = sizeof(WNDCLASSEX);
	// use default class styles
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// WindowProc �Լ��� lpfnWndProc�� �����մϴ�.
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	// IDCICON_SMALL,	//we're using a small icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// background color for the window
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	// class name in the window class
	wc.lpszClassName = L"WindowClass";

	//register the window class
	RegisterClassEx(&wc);

	RECT wr = { 0,0,500,400 }; // set the size, but not the position

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE); // adjust the size

	//Create the window and use the result as the handle
	/*
	hwnd = CreateWindowEx(NULL,
		L"WindowClass",						//name of the window class
		L"Our First Direct3D Program",	//title of the window
		WS_OVERLAPPEDWINDOW,			//window style
		300,							//x-position of the window
		300,							//y-position of the window
		SCREEN_WIDTH,							//width of the window
		SCREEN_HEIGHT,							//height of the window
		NULL,							//we have no parent window, NULL
		NULL,							//we aren't using menus, NULL
		hInstance,						//application handle
		NULL);							//used with multiple windows, NULL
	*/


	hwnd = CreateWindowEx(NULL,
		L"WindowClass",					//name of the window class
		L"Our First Direct3D Program",	//title of the window
		WS_OVERLAPPEDWINDOW,		//window style
		0,								//x-position of the window
		0,								//y-position of the window
		SCREEN_WIDTH,					//width of the window
		SCREEN_HEIGHT,					//height of the window
		NULL,							//we have no parent window, NULL
		NULL,							//we aren't using menus, NULL
		hInstance,						//application handle
		NULL);							//used with multiple windows, NULL

	//display the window on the screen
	//ȭ�鿡 �����츦 ǥ���Ѵ�.
	ShowWindow(hwnd, nShowCmd);

	//set up and initialize Direct3D
	initD3D(hwnd);
	
	//enter the main loop;

	//this struct holds windows event messages
	//�� ����ü�� ������ �̺�Ʈ �޽����� �����մϴ�.
	MSG msg;

	//wait for the next message in the queue, store the result in 'msg'
	//ť���� ���� �޽����� ��ٸ��� , ����� 'msg'�� �����մϴ�.
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // translate keystroke messages into the right format
			DispatchMessage(&msg); // send the message to the WindowProc function
		}

		if (msg.message == WM_QUIT) // check if the message is a quit message
		{
			break; // if so, exit the loop
		}
		render_frame(); // otherwise, continue rendering frames
	}

	//clean up Direct3D and COM
	cleanD3D();

	//return this part of the WM_QUIT message it  Windows
	return msg.wParam;
}


// this is the main message to the WindowProc function
// �̰��� WindowProc �Լ��� ���� ���Ǻ��Դϴ�.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//sort through and find what cide to run for the massage
	//�޽����� ���� � �ڵ带 �������� �����մϴ�.
	switch (uMsg)
	{
		//this message is read when the window is closed
		//�� �޽����� â�� ������ �о���� �޽����Դϴ�.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}
	//Handle any messages the switch statement didn't
	//�ڵ��� ����ġ ������ ó������ ���� ��� �����մϴ�.
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp)); // clear out the struct for use
	d3dpp.Windowed = TRUE; // program will be in windowed mode
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames	
	d3dpp.hDeviceWindow = hWnd; // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // set the back buffer format
	d3dpp.BackBufferWidth = SCREEN_WIDTH; // set the width of the buffer
	d3dpp.BackBufferHeight = SCREEN_HEIGHT; // set the height of the buffer

	//create a device class using this information and information from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	init_graphics(); // call the function to initialize the triangle
}

void render_frame(void)
{
	//clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene(); // begins the scene

	//select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);
	//select the vertex buffer to display
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	//copy the vertex buffer to the back buffer
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// do 3D rendering on the back buffer here

	d3ddev->EndScene(); // ends the scene

	d3ddev->Present(NULL, NULL, NULL, NULL); // displays the created frame on the screen

}

//this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	v_buffer->Release(); // close and release the vertex buffer
	d3ddev->Release(); // close and release the 3D device
	d3d->Release(); // close and release Direct3D
}

void init_graphics(void) 
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ 400.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
		{ 650.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 150.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
	};

	d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);
	VOID* pVoid; // a void pointer

	// lock the vertex buffer so we can fill it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices)); // copy the vertices into the buffer
	v_buffer->Unlock(); // unlock the vertex buffer when done
}