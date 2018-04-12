//
//  auther : JYLEELYJ @ https://github.com/JYLeeLYJ 
//
//	main_framework.cpp : win32 window initialize 
//

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#include <windows.h>
#include <tchar.h>
#include <cstdio>

#include "Tiny3D.h"


#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

using t3d::T3DDevice;

/***********************************************************
* global configuration
************************************************************/

const int WIDTH		= 800;
const int HEIGHT	= 600;

extern void Display();

/******************************************************
 * win32 init
 *****************************************************/

inline	void		update(HWND);

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int,HWND &);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

	HWND hWnd;

	if (!InitInstance(hInstance, nCmdShow,hWnd))return FALSE;

    MSG msg;

    // 主消息循环: 
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  
		{
			TranslateMessage(&msg);
			if (msg.message == WM_QUIT)break;
			DispatchMessage(&msg);
		}

		//TODO:
		//tNow = GetTickCount();
		//if (tNow - tPre >= 40) {
		Display();
		HDC hdc = GetDC(hWnd);
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, T3DDevice::GetDevice()->get_cdc(), 0, 0, SRCCOPY);
		//BitBlt(hdc, 0, 0, WIDTH, HEIGHT, device.get_cdc(), 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdc);
	}

    return (int) msg.wParam;
}

/* 注册窗口结构体 */
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
	wcex.hIcon			= nullptr;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= nullptr;// (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = nullptr;
	wcex.lpszClassName	= L"TINY3D";
    wcex.hIconSm        = nullptr;

    return RegisterClassExW(&wcex);
}

/* 初始化 */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow,HWND & hWnd)
{
   //hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindowW(L"Tiny3D", L"Tiny3D", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
      100, 100, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)return FALSE;

   //TODO:device initialize 
   //device.Init(hWnd, WIDTH, HEIGHT);
   T3DDevice::GetDevice()->Init(hWnd, WIDTH, HEIGHT);

   //MoveWindow(hWnd, 400, 400, WIDTH, HEIGHT, TRUE);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/* 消息处理 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_KEYDOWN:

        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			Display();
			BitBlt(hdc, 0, 0, WIDTH, HEIGHT, T3DDevice::GetDevice()->get_cdc(), 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

inline void update(HWND hwnd)
{
	
}