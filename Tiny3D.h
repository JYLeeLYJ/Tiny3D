#pragma once

#include <Windows.h>

namespace t3d
{
	typedef unsigned int uint;
	typedef unsigned char uchar;

	typedef unsigned int Color;

	/* abstract device */
	class T3DDevice
	{
		//T3DDevice() { };

		//static T3DDevice *_instance;	// single instance

		void *			_bmpbit;		// get from CreateDIBSection function
		uint **			_framebuf;		// framebuffer pointer
		HBITMAP			_dib_handle;	// return from CreateDIBSection function 
		HBITMAP			_obj_handle;	// return from SelectObject function
		HDC				_cdc;			// compatible dc
		BITMAPINFO		_bmp_info;		// bitmap info struct
		uint			_width;
		uint			_height;
		
		//get hinstance from GetModuleHandle 

	public:

		//single instance (single_thread.ver)
		//static T3DDevice* GetDevice() { return (_instance ? _instance : _instance=new T3DDevice); }

		void Init(HWND,int,int );
		HDC & get_cdc() { return _cdc; }

		//test function
		void reset_buffer();

	};

}
