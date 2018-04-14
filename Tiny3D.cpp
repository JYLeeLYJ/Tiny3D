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

	_view.SetIdentity();
	_world.SetIdentity();
	_proj.SetIdentity();

	_state = 1;

	//TODO:other members init : z buffer;

}

void T3DDevice::BufferReset()
{
	memset(_bmpbit, 0xff, _width*_height * 4);

	//TODO: BufferReset() : z buffer clear 

	/*test funtion */
	//for (int i = 200; i < 400; i++)
	//	for (int j = 300; j < 500; j++)
	//		_framebuf[i][j]= 0xff002bff;	//note that the color data format in GDI is BGR ,not RGB
}

void T3DDevice::SetTransform(T3DMatrix &mat, Transform t) {

	switch (t)
	{
	case t3d::WORLD:
		_world = mat;
		break;
	case t3d::VIEW:
		_view = mat;
		break;
	case t3d::PROJECTION:
		_proj = mat;
		break;
	default:
		break;
	}
}

// view transform  ,note that I use left-handed coordinate in the transformation computing 
void T3DDevice::SetLookAtLH(T3DVector eye, T3DVector up, T3DVector target) {

	//右乘列向量，先平移再旋转摄像机坐标轴
	T3DVector &p = eye;
	T3DVector v = target - eye;				//z
	v.normalize();
	T3DVector &u = up;						//y
	u.normalize();
	T3DVector r = T3DVector::cross(u, v);	//x		左手系 x = y * z 
	u = T3DVector::cross(v, r);		//再次叉积保证向量正交

	_view._mat[3][0] = _view._mat[3][1] = _view._mat[3][2] = 0.0f;
	_view._mat[0][0] = r._x; _view._mat[0][1] = r._y; _view._mat[0][2] = r._z;
	_view._mat[1][0] = u._x; _view._mat[1][1] = u._y; _view._mat[1][2] = u._z;
	_view._mat[2][0] = v._x; _view._mat[2][1] = v._y; _view._mat[2][2] = v._z;
	_view._mat[0][3] = -r * p; 
	_view._mat[1][3] = -u * p;
	_view._mat[2][3] = -v * p;
	_view._mat[3][3] = 1.0f;

}

// c/a	0	0	0
//	0	c	0	0
//	0	0 f/(f-n) -fn/(f-n)
//	0	0	1	0
void T3DDevice::SetPerspectiveFovLH(float fov, float aspect, float zn, float zf) {

	float cot = 1.0f /tan(fov * 0.5f);

	_proj._mat[0][0] = cot / aspect;
	_proj._mat[1][1] = cot;
	_proj._mat[2][2] = zf / (zf - zn);
	_proj._mat[2][3] = -zn * zf / (zf - zn);
	_proj._mat[3][2] = 1;
	_proj._mat[3][3] = _proj._mat[3][1] = _proj._mat[3][0] = 0.0f;
	_proj._mat[2][0] = _proj._mat[2][1] = 0.0f;
	_proj._mat[1][0] = _proj._mat[1][2] = _proj._mat[1][3] = 0.0f;
	_proj._mat[0][1] = _proj._mat[0][2] = _proj._mat[0][3] = 0.0f;
}

void T3DDevice::DrawIndexPrimitive(Primitive p,uint num_vertices,uint cnt_primitive) {

	uint n_indices = 3 * cnt_primitive;

	// 图元装配·伪
	if (p == TRIANGLE)
		for (uint i = 0; i < n_indices; i += 3)
		{
			auto &v1 = _vb[_ib[i]];
			auto &v2 = _vb[_ib[i + 1]];
			auto &v3 = _vb[_ib[i + 2]];
			draw_triangle(v1, v2, v3);
			//draw_triangle(_vb[_ib[i]], _vb[_ib[i + 1]], _vb[_ib[i + 2]]);
		}
	//TODO: other primitive drawing implementation
}

void T3DDevice::draw_triangle(Vertex v1, Vertex v2, Vertex v3) {

	_transform = _proj * _view * _world;
	
	//from world to cvv 
	v1._pos = _transform * v1._pos;
	v2._pos = _transform * v2._pos;
	v3._pos = _transform * v3._pos;

	//simple clipping in cvv space
	//只要有一个点在体外则整面删除
	//更详细的裁剪可以将面分割，再得到新的三角形
	if (simple_cvv_test(v1._pos) | simple_cvv_test(v2._pos)|
		simple_cvv_test(v3._pos))
		return;

	//TODO:表面剔除
	//T3DVector normal=cross()

	//将坐标归一化并进行屏幕映射
	//依然保留有深度信息在_z 中 [0,1]
	screen_map(homogenize(v1._pos));
	screen_map(homogenize(v2._pos));
	screen_map(homogenize(v3._pos));

	//三角形光栅化
	rasterize(v1, v2, v3);

}

bool T3DDevice::simple_cvv_test(Point &p) {

	float w = p._w;

	//顶点在视体外
	if (p._z < 0.0f | p._z > w |
		p._x < -w | p._x >  w |
		p._y < -w | p._y > w)
		return true;

	return false;

}

Point & T3DDevice::homogenize(Point &p) {
	p._x /= p._w;
	p._y /= p._w;
	p._z /= p._w;
	p._w = 1.0f;
	return p;
}

void T3DDevice::screen_map(Point &p) {
	p._x = (p._x + 1.0f)*_width / 2;
	p._y = (1.0f - p._y)*_height / 2;
}

inline void T3DDevice::rasterize(Vertex& v1, Vertex& v2, Vertex& v3)
{
	//线框模式
	if (_state = WIREFRAME)
	{
		//bresenham(v1, v2, 0);
		//bresenham(v1, v3, 0);
		//bresenham(v2, v3, 0);
		draw_line_test(v1._pos._x, v1._pos._y, v2._pos._x, v2._pos._y, 0);
		draw_line_test(v1._pos._x, v1._pos._y, v3._pos._x, v3._pos._y, 0);
		draw_line_test(v2._pos._x, v2._pos._y, v3._pos._x, v3._pos._y, 0);
	}

	//TODO: 纹理映射，平面
}

//Bresenham 直线光栅化
//需要注意斜率不存在的情况——垂直直线
void T3DDevice::bresenham(Vertex &v0, Vertex &v1, uint color) {

	int x0, y0, x1, y1;
	if (v0._pos._x < v1._pos._x) {
		x1 = (int)v1._pos._x; y1 = (int)v1._pos._y;
		x0 = (int)v0._pos._x; y0 = (int)v0._pos._y;
	}
	else {
		x0 = (int)v1._pos._x; y0 = (int)v1._pos._y;
		x1 = (int)v0._pos._x; y1 = (int)v0._pos._y;
	}


	int dx = x1 - x0;
	int dy = y1 - y0;
	
	int x = x0, y = y0;
	int e = -dx;

	for (int i = 0; i <= dx; ++i) {
		draw_pixel(x, y, color);
		++x;
		e += 2 * dy;
		if (e >= 0) {
			++y;
			e -= 2 * dx;
		}
	}

	//
	if (dx == 0) {
		for (y = y0; y <= y1; ++y)draw_pixel(x, y, color);
	}
}


//	skywind mini3d.c - draw_line ()function
//	github.com/skywind3000/mini3d
void T3DDevice::draw_line_test(int x1, int y1, int x2, int y2, uint c) {

	int x, y, rem = 0;
	if (x1 == x2 && y1 == y2) {
		draw_pixel( x1, y1, c);
	}
	else if (x1 == x2) {
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc) draw_pixel(x1, y, c);
		draw_pixel( x2, y2, c);
	}
	else if (y1 == y2) {
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc) draw_pixel( x, y1, c);
		draw_pixel(x2, y2, c);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				draw_pixel(x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					draw_pixel(x, y, c);
				}
			}
			draw_pixel( x2, y2, c);
		}
		else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				draw_pixel( x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					draw_pixel(x, y, c);
				}
			}
			draw_pixel( x2, y2, c);
		}
	}
}

