#pragma once

#include <cmath>

namespace tnm
{
	/********************************************
	 *	4 by 1 matrix(vector)  
	 ********************************************/
	class Vector4 {
	public :

		Vector4() { _w = 1; }
		Vector4(float x, float y, float z) { _x = x; _y = y; _z = z; _w = 1; };

		Vector4 operator+(const Vector4 &rhs);
		Vector4 operator-(const Vector4 &rhs);
		Vector4 operator-() { return Vector4(-_x, -_y, -_z); };
		
		//dot product
		float operator*(const Vector4 &rhs) { return this->_x*rhs._x + this->_y *rhs._y + this->_z*rhs._z; }
		static float dot(const Vector4 &lhs, const Vector4 &rhs) { return lhs._x*rhs._x + lhs._y*rhs._y + lhs._z*rhs._z; };	

		//cross_product
		static Vector4	cross(const Vector4 &,const Vector4 &);		

		float norm() { return sqrt(this->_x*this->_x + this->_y*this->_y + this->_z*this->_z); };
		Vector4&  normalize() { float norm = this->norm(); _x /= norm; _y /= norm; _z /= norm; return *this; };

		float _x, _y, _z, _w;
	};

	/*************************************************
	 *	4 by 4 matrix 
	 *************************************************/
	class Matrix4 {

	public:

		Matrix4() {};

		void SetIdentity() {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j) {
					if (i == j)_mat[i][j] = 1.0f;
					else _mat[i][j] = 0.0f;
				}
		}

		Matrix4 operator+(const Matrix4&);
		Matrix4 operator-(const Matrix4&);
		Matrix4	operator *(const Matrix4& rhs);
		Vector4 operator*(const Vector4&);	// M * v

		Matrix4& SetRotationQ( float ,float,float,float );

		Matrix4& SetRotationX(float);
		Matrix4& SetRotationY(float);
		Matrix4& SetRotationZ(float);

		float _mat[4][4];

	};
}