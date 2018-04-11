//
//  auther : JYLEELYJ @ https://github.com/JYLeeLYJ 
//

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#include <windows.h>
#include <tchar.h>

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

/***********************************************************
* 设置数据
************************************************************/
// 全局变量: 当前实例
HINSTANCE hInst;    
const int wWidth	= 800;
const int wHeight	= 600;


// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


/******************************************************
 * win32 窗口初始化
 *****************************************************/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))return FALSE;

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
		else {
			//tNow = GetTickCount();//获取时间

			//时间差大于40ms 绘制一帧
			//if (tNow - tPre >= 40) {
			display();
		}
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(L"Tiny3D", L"Tiny3D", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)return FALSE;

   MoveWindow(hWnd, 400, 400, wWidth, wHeight, TRUE);
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			display();
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

void display()
{

}