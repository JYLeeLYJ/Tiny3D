#pragma once

namespace
{

	class Vector3 {
	public :

		Vector3(float x, float y, float z) { this->_x = x; this->_y = y; this->_z = z; };

		Vector3& operator+(const Vector3 &);	//add
		Vector3& operator-(const Vector3 &);	//sub

		friend Vector3 cross(const Vector3 &);			//cross_product
		friend Vector3 dot(const Vector3 &, const Vector3 &);	//dot product

		//Vector3& operator*(const Matrix4&);
	
	private :
		float _x, _y, _z;
	};

	typedef Vector3 Vertice;


	class Matrix4 {

		Matrix4(float **mat);

		Matrix4& operator+(const Matrix4&);
		Matrix4& operator-(const Matrix4&);

		friend void dot(const Matrix4 &, const Matrix4 &);


	};
}