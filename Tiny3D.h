#pragma once

#include <Windows.h>

#include "tinymat.h"


namespace t3d
{

	typedef unsigned int uint;
	typedef unsigned char uchar;

	typedef tnm::Matrix4	T3DMatrix;
	typedef tnm::Vector4	T3DVector;
	typedef tnm::Vector4	Point;

	/* 
	 *	type	:	Transform enum 
	 *	member	:	world , view , projection
	 */
	enum Transform { WORLD, VIEW, PROJECTION };

	/* 
	 *	type	:	Primitive enum 
	 *	member	:	point , line , triangle
	 */
	enum Primitive { POINT, LINE, TRIANGLE };

	/* 
	 *	type	:	Render state
	 *	member	:	wireframe , texture , color
	 */
	enum RenderState { WIREFRAME=1, TEXTRUE=2, COLOR=4,BACKFACE_CULLING=8 };

	/***************************************
	 *	class name	:	Vertex
	 *	note		:	
	 ***************************************/
	class Vertex
	{
	public :
		//TODO:Vertex FUNCTIONS
		Vertex() { };
		Vertex(float x, float y, float z) :_pos(x, y, z) { _color = 0xffffffff; };

		Point _pos;
		union {
			uint _color;
			uchar _bgra[4];
		};
	};

	typedef Vertex			*T3DVertexBuffer;
	typedef uint			*T3DVertexIndex;

	/* *************************************
	 *	class name	:	T3DDevice
	 *	note		:	abstract device 
	 ***************************************/
	class T3DDevice
	{
	private:

		//private declaration for singleton   
		T3DDevice() { };

		static T3DDevice *_instance;	// single instance

		void *			_bmpbit;		// assigned in CreateDIBSection function
		uint **			_framebuf;		// framebuffer pointer
		float			_zbuffer;		
		HBITMAP			_dib_handle;	// return from CreateDIBSection function 
		HBITMAP			_obj_handle;	// return from SelectObject function
		HDC				_cdc;			// compatible dc
		BITMAPINFO		_bmp_info;		// bitmap info struct
		uint			_width;
		uint			_height;

		T3DVertexBuffer	_vb;	//vertex buffer
		T3DVertexIndex	_ib;	//vertex index 

		T3DMatrix		_world;		//world Transform Matrix 
		T3DMatrix		_view;		//view transform
		T3DMatrix		_proj;		//proj transform
		T3DMatrix		_transform;	//_proj * _view * _world

		uint			_state;	//render state
		
		//you can get hinstance using GetModuleHandle() API 

		//Geometry stage 
		void draw_triangle(Vertex p1, Vertex p2, Vertex p3);
		bool simple_cvv_test(Point &p);
		inline Point& homogenize(Point &p);
		inline void screen_map(Point &p);

		//Rasterization stages
		inline void rasterize(Vertex &v1, Vertex &v2,Vertex &v3);
		void bresenham(Vertex &v0, Vertex &v1, uint color);
		void draw_line_test(int x0, int y0, int x1, int y1, uint color);
		void draw_pixel(int x, int y, uint color) { _framebuf[y][x] = color; };

	public:

		void Init(HWND hwnd, int w, int h);
		static T3DDevice* GetDevice() { return (_instance ? _instance : _instance=new T3DDevice); }

		HDC & GetCDC() { return _cdc; }
		uint GetWidth() { return _width; }
		uint GetHeight() { return _height; }

		void BufferReset();

		T3DVertexBuffer  CreateVertexBuffer(uint n) { return new Vertex[n]; }
		T3DVertexIndex  CreateIndexBuffer(uint n) { return new uint[n]; }

		void SetLookAtLH(T3DVector eye, T3DVector up, T3DVector target);
		void SetPerspectiveFovLH(float fovy,float aspect,float z_near,float z_far);
		void SetIndices(T3DVertexIndex ib) { _ib= ib; };
		void SetStreamSourse(T3DVertexBuffer vb) { _vb = vb; }
		void SetTransform(T3DMatrix &mat, Transform t);
		void SetRenderState(uint state) { _state = state; }

		void DrawIndexPrimitive(Primitive primitive_type,uint num_vertex,uint num_index);
	};

}
