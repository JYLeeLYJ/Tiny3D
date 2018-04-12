#include<cstring>
#include<cassert>

#include "Tiny3D.h"

using namespace t3d;

T3DDevice * T3DDevice::_instance = nullptr;

void T3DDevice::Init(HWND hwnd, int w, int h)
{
	assert(hwnd);

	HDC hdc = GetDC(hwnd);
	_cdc = CreateCompatibleDC(hdc);
	ReleaseDC(hwnd, hdc);

	_bmp_info = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,(DWORD)w * h * 4, 0, 0, 0, 0 } };

	_dib_handle = CreateDIBSection(_cdc,&_bmp_info, DIB_RGB_COLORS, &_bmpbit,NULL,NULL);
	_obj_handle = (HBITMAP)SelectObject(_cdc, _dib_handle);

	_framebuf = new uint*[h];	
	for (int i = 0; i < h; ++i) {
		_framebuf[i] = (uint *)((char *)_bmpbit + i * w * 4);
	}

	_width = w;
	_height = h;

	//TODO:other members init

}


void T3DDevice::BufferReset()
{
	memset(_bmpbit, 0, _width*_height * 4);

	/*test funtion */
	for (int i = 200; i < 400; i++)
		for (int j = 300; j < 500; j++)
			_framebuf[i][j]= 0xff002bff;	//note that the color data format in GDI is BGR ,not RGB
}