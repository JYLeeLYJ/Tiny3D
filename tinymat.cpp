
#include "tinymat.h"

using namespace tnm;

//***********************************************
// Vector4 implementation
//***********************************************

Vector4 Vector4::operator+(const Vector4 &rhs) {

	Vector4 v;
	v._x = this->_x + rhs._x;
	v._y = this->_y + rhs._y;
	v._z = this->_z + rhs._z;
	return v;

}


Vector4 Vector4::operator-(const Vector4 &rhs) {

	Vector4 v;
	v._x = this->_x - rhs._x;
	v._y = this->_y - rhs._y;
	v._z = this->_z - rhs._z;
	return v;

}

Vector4 Vector4::cross(const Vector4 &lhs, const Vector4 &rhs) {
	
	Vector4 v;

	v._x = lhs._y*rhs._z - lhs._z*rhs._y;
	v._y = lhs._z*rhs._x - lhs._x*rhs._z;
	v._z = lhs._x*rhs._y - lhs._y*rhs._x;
	return v;
	
}

//************************************************
// Matrix4 implementation
//************************************************

Matrix4 Matrix4::operator+(const Matrix4& m) {

	Matrix4 mat;
	for (int i=0;i<4;++i)
		for (int j = 0; i < 4; ++j) {
			mat._mat[i][j] = this->_mat[i][j] + m._mat[i][j];
		}
	return mat;
}

Matrix4 Matrix4::operator-(const Matrix4& m)
{
	Matrix4 mat;
	for (int i = 0; i<4; ++i)
		for (int j = 0; i < 4; ++j) {
			mat._mat[i][j] = this->_mat[i][j] - m._mat[i][j];
		}
	return mat;
}

// this * m 
Matrix4 Matrix4::operator*(const Matrix4& m)
{
	Matrix4 mat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			mat._mat[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				mat._mat[i][j] += this->_mat[i][k] * m._mat[k][j];
			}
		}
	return mat;
}

// mat * v
Vector4 Matrix4::operator* (const Vector4& v)
{
	Vector4 vtnmp;
	vtnmp._x = v._x*_mat[0][0] + v._y*_mat[0][1] + v._z*_mat[0][2] + _mat[0][3];
	vtnmp._y = v._x*_mat[1][0] + v._y*_mat[1][1] + v._z*_mat[1][2] + _mat[1][3];
	vtnmp._z = v._x*_mat[2][0] + v._y*_mat[2][1] + v._z*_mat[2][2] + _mat[2][3];
	vtnmp._w = v._x*_mat[3][0] + v._y*_mat[3][1] + v._z*_mat[3][2] + _mat[3][3];
	return vtnmp;
}

//build up rotation matrix using Quaternion
Matrix4& Matrix4::SetRotationQ(float x,float y,float z,float theta) {

	//ref : skywind mini3d.c matrix_set_rotate function 

	float qsin = sin(theta * 0.5f);
	float qcos = cos(theta * 0.5f);
	float w = qcos;

	Vector4 v(x,y,z);
	v.normalize();

	x = v._x * qsin;
	y = v._y * qsin;
	z = v._z * qsin;

	_mat[0][0] = 1 - 2 * y * y - 2 * z * z;
	_mat[0][1] = 2 * x * y - 2 * w * z;
	_mat[0][2] = 2 * x * z + 2 * w * y;
	_mat[1][0] = 2 * x * y + 2 * w * z;
	_mat[1][1] = 1 - 2 * x * x - 2 * z * z;
	_mat[1][2] = 2 * y * z - 2 * w * x;
	_mat[2][0] = 2 * x * z - 2 * w * y;
	_mat[2][1] = 2 * y * z + 2 * w * x;
	_mat[2][2] = 1 - 2 * x * x - 2 * y * y;
	_mat[0][3] = _mat[1][3] = _mat[2][3] = 0.0f;
	_mat[3][0] = _mat[3][1] = _mat[3][2] = 0.0f;
	_mat[3][3] = 1.0f;

	return *this;
}

//	1	0	0	0
//	0	cos	-sin0
//	0   sin	cos	0
//	0	0	0	1
Matrix4& Matrix4::SetRotationX(float angle) {

	float c = cos(angle);
	float s = sin(angle);

	_mat[0][0] = _mat[3][3] = 1;
	_mat[1][1] = _mat[2][2] = c;
	_mat[2][1] = s; _mat[1][2] = -s;

	_mat[0][1] = _mat[0][2] = _mat[0][3] = 0.0f;
	_mat[1][0] = _mat[1][3] = 0.0f;
	_mat[2][0] = _mat[2][3] = 0.0f;
	_mat[3][0] = _mat[3][1] = _mat[3][2] = 0.0f;
	
	return *this;
}

//	cos	0   sin	0
//	0	1	0	0
// -sin	0	cos	0
//	0	0	0	1
Matrix4& Matrix4::SetRotationY( float angle) {

	float c = cos(angle);
	float s = sin(angle);

	_mat[0][0] = _mat[2][2] = c;
	_mat[1][1] = _mat[3][3] = 1;
	_mat[0][2] = s; _mat[2][0] = -s;

	_mat[0][1] = _mat[0][3] = 0.0f;
	_mat[1][0] = _mat[1][2] = _mat[1][3] = 0.0f;
	_mat[2][1] = _mat[2][3] = 0.0f;
	_mat[3][0] = _mat[3][1] = _mat[3][2] = 0.0f;

	return *this;
}

//	cos	sin	0	0
// -sin	cos	0	0
//	0	0	1	0
//	0	0	0	1
Matrix4& Matrix4::SetRotationZ( float angle) {

	float c = cos(angle);
	float s = sin(angle);

	_mat[0][0] = _mat[1][1] = c;
	_mat[2][2] = _mat[3][3] = 1.0f;
	_mat[1][0] = s; _mat[0][1] = -s;

	_mat[0][2] = _mat[0][3] = 0.0f;
	_mat[2][0] = _mat[2][1] = _mat[2][3] = 0.0f;
	_mat[1][2] = _mat[1][3] = 0.0f;
	_mat[3][0] = _mat[3][1] = _mat[3][2] = 0.0f;

	return *this;
}



