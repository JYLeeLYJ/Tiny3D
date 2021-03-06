//******************************************************
//  auther		: JYLEELYJ in github.com/JYLeeLYJ 
//	name		: main_framework.cpp
//	description : win32 window initialize 
//******************************************************

#include <windows.h>
#include <tchar.h>
#include <cstdio>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h> 
#include <crtdbg.h>
#endif // _DEBUG


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
extern void Setup();
extern void OnKeyUp(UINT , WPARAM , LPARAM );

/******************************************************
 * win32 init
 *****************************************************/

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
	int t_now, t_pre ,t_second;
	WCHAR text_name[20];
	t_second = GetTickCount();

    // 主消息循环: 
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  
		{
			TranslateMessage(&msg);
			if (msg.message == WM_QUIT)break;
			DispatchMessage(&msg);
		}

		t_pre = GetTickCount();

		//*******************************
		//		绘图区域代码
		//
		Display();
		HDC hdc = GetDC(hWnd);
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, T3DDevice::GetDevice()->GetCDC(), 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdc);
		//*******************************

		Sleep(15);
		t_now = GetTickCount();


		//每秒刷新fps
		if (t_now - t_second > 1000) {
			swprintf(text_name, L"Tiny3D - FPS:%d", 1000 / (t_now - t_pre));
			t_second = GetTickCount();
			SetWindowText(hWnd, text_name);
		}
	
	}

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

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
   hWnd = CreateWindowW(L"Tiny3D", L"Tiny3D", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
      100, 100, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)return FALSE;

   T3DDevice::GetDevice()->Init(hWnd, WIDTH, HEIGHT);
   Setup();

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
		OnKeyUp(message, wParam, lParam);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			Display();
			BitBlt(hdc, 0, 0, WIDTH, HEIGHT, T3DDevice::GetDevice()->GetCDC(), 0, 0, SRCCOPY);
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
