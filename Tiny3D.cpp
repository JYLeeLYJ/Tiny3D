#include<cstring>
#include<cassert>

#include "Tiny3D.h"

using namespace t3d;

//T3DDevice * _instance = nullptr;

void T3DDevice::Init(HWND hwnd, int w, int h)
{
	assert(hwnd);

	HDC hdc = GetDC(hwnd);
	_cdc = CreateCompatibleDC(hdc);
	ReleaseDC(hwnd, hdc);

	_bmp_info = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,(DWORD)w * h * 4, 0, 0, 0, 0 } };

	_dib_handle = CreateDIBSection(_cdc,&_bmp_info, DIB_RGB_COLORS, &_bmpbit,NULL,NULL);
	_obj_handle = (HBITMAP)SelectObject(_cdc, _dib_handle);

	//char *buf = (char *)ptr;
	//_framebuf = static_cast<uint *>(ptr);
	_framebuf = new uint*[h];	
	for (int i = 0; i < h; ++i) {
		_framebuf[i] = (uint *)((char *)_bmpbit + i * w * 4);
	}

	_width = w;
	_height = h;

	//TODO:other members init

}


void T3DDevice::reset_buffer()
{
	memset(_bmpbit, 0, _width*_height * 4);

	/*test funtion */
	int begw = _width / 4;
	int begh = _height / 4;
	for (int i = begh; i < 3 * begh; i++)
		for (int j = begw; j < 3 * begw; j++)
			_framebuf[i][j]= 0xff002bff;
			//*(_framebuf[i]+j) = 0xff002bff;
			//*(_framebuf+_width*i+j ) = 0xff002bff;	//note that the color format in GDI is BGR ,not RGB
}