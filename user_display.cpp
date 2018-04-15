#include "Tiny3D.h"

using namespace t3d;

#define PI 3.1415926f

static float theta = 0.0f;

void Setup()
{
	auto device = T3DDevice::GetDevice();

	auto VB = device->CreateVertexBuffer(8);
	auto IB = device->CreateIndexBuffer(36);

	// vertices of a cube


	VB[0] = Vertex(-1.0f, -1.0f, -1.0f);
	VB[1] = Vertex(-1.0f,  1.0f, -1.0f);
	VB[2] = Vertex( 1.0f,  1.0f, -1.0f);
	VB[3] = Vertex( 1.0f, -1.0f, -1.0f);
	VB[4] = Vertex(-1.0f, -1.0f,  1.0f);
	VB[5] = Vertex(-1.0f,  1.0f,  1.0f);
	VB[6] = Vertex( 1.0f,  1.0f,  1.0f);
	VB[7] = Vertex( 1.0f, -1.0f,  1.0f);

	uint Indices[] = { 0,1,2,
		0,2,3,
		4,6,5,
		4,7,6,
		4,5,1,
		4,1,0,
		3,2,6,
		3,6,7,
		1,5,6,
		1,6,2,
		4,0,3,
		4,3,7 };


	for (int i = 0; i < 36; ++i) {
		IB[i] = Indices[i];
	}

	/*
	VB[0] = Vertex(0.0f, 0.0f, 0.0f);
	VB[1] = Vertex(1.0f, 0.0f, 0.0f);
	VB[2] = Vertex(0.0f, 1.0f, 1.0f);
	
	uint Indices[] = { 0,1,2 };*/
	
	T3DVector eye(0.0f, 0.0f, -3.0f);
	T3DVector target(0.0f, 0.0f, 0.0f);
	T3DVector up(0.0f, 1.0f, 0.0f);

	uint w = device->GetWidth();
	uint h = device->GetHeight();

	//set _view and _proj matrix
	device->SetLookAtLH(eye, up, target);
	device->SetPerspectiveFovLH(PI*0.5f, float(w) / h, 1.0f, 1000.f);

	device->SetRenderState(WIREFRAME | BACKFACE_CULLING);	//线框模式 | 背面剔除

	device->SetIndices(IB);
	device->SetStreamSourse(VB);

	//TODO:set texture data

}

void Display() {

	auto device = T3DDevice::GetDevice();
	
	T3DMatrix m;

	//设置旋转轴,利用四元数
	m.SetRotationQ(-1.0f, -0.5f, 1.0f, theta);

	//级联旋转
	//T3DMatrix mx, my;
	//mx.SetRotationX(PI / 4.0f);
	//my.SetRotationY(theta);
	//m = my * mx;
	
	device->SetTransform(m, WORLD);
	device->BufferReset();

	//draw cube 
	device->DrawIndexPrimitive(TRIANGLE,8,12);
	//device->bresenham(Vertex(300, 300, 0), Vertex(400, 200, 0),0);

}

void OnKeyUp(UINT message, WPARAM wParam, LPARAM lParam) {

	theta += 0.05f;
	if (theta > 6.28)theta = 0;

}