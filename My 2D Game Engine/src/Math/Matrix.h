#pragma once
#include <numbers>
#include "Vector2.h";

template<typename T, size_t SIZE>
struct Matrix
{
	T matrix[SIZE][SIZE];

	Matrix();
	Matrix(T diagonal);

	//Matrix(const Matrix& other) = default;

	template<size_t S>
	Matrix(const Matrix<T, S>& other);

	template<size_t S>
	const Matrix<T, SIZE>& operator=(const Matrix<T, S>& other);

	template<typename U>
	void operator+=(U scalar);
	template<typename U>
	void operator+=(const Matrix<U, SIZE>& m);

	template<typename U>
	void operator-=(U scalar);
	template<typename U>
	void operator-=(const Matrix<U, SIZE>& m);

	template<typename U>
	void operator*=(U scalar);
	template<typename U>
	void operator*=(const Matrix<U, SIZE>& m);

	T* operator[](size_t index);
	const T* operator[](size_t index) const;

	Matrix<T, SIZE> Transpose() const;
};

template<typename T, size_t SIZE>
inline Matrix<T, SIZE>::Matrix()
	: matrix{ 0 }
{
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE>::Matrix(T diagonal)
	: matrix { 0 }
{
	for (size_t i = 0; i < SIZE; i++)
		matrix[i][i] = diagonal;
}

template<typename T, size_t SIZE>
template<size_t S>
inline Matrix<T, SIZE>::Matrix(const Matrix<T, S>& other)
{
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			matrix[i][j] = other[i][j];
	}
}

template<typename T, size_t SIZE>
template<size_t S>
inline const Matrix<T, SIZE>& Matrix<T, SIZE>::operator=(const Matrix<T, S>& other)
{
	if (S < SIZE)
		return;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			matrix[i][j] = other[i][j];
	}

	return *this;
}

template<typename T, size_t SIZE>
template<typename U>
inline void Matrix<T, SIZE>::operator+=(U scalar)
{
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			matrix[i][j] += scalar;
	}
}

template<typename T, size_t SIZE>
template<typename U>
inline void Matrix<T, SIZE>::operator+=(const Matrix<U, SIZE>& m)
{
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			matrix[i][j] += m[i][j];
	}
}

template<typename T, size_t SIZE>
template<typename U>
inline void Matrix<T, SIZE>::operator-=(U scalar)
{
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			matrix[i][j] -= scalar;
	}
}

template<typename T, size_t SIZE>
template<typename U>
inline void Matrix<T, SIZE>::operator-=(const Matrix<U, SIZE>& m)
{
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			matrix[i][j] -= m[i][j];
	}
}

template<typename T, size_t SIZE>
template<typename U>
inline void Matrix<T, SIZE>::operator*=(U scalar)
{
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			matrix[i][j] *= scalar;
	}
}

template<typename T, size_t SIZE>
template<typename U>
inline void Matrix<T, SIZE>::operator*=(const Matrix<U, SIZE>& m)
{
	*this = *this * m;
}

template<typename T, size_t SIZE>
inline T* Matrix<T, SIZE>::operator[](size_t index)
{
	return matrix[index];
}

template<typename T, size_t SIZE>
inline const T* Matrix<T, SIZE>::operator[](size_t index) const
{
	return matrix[index];
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> Matrix<T, SIZE>::Transpose() const
{
	Matrix<T, SIZE> transpose;
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			transpose[i][j] = matrix[j][i];
	}

	return transpose;
}

//-------------------------------------------
// -- Binary operators --
template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator+(const Matrix<T, SIZE>& m, T scalar)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			temp[i][j] = m[i][j] + scalar;
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator+(const Matrix<T, SIZE>& m1, const Matrix<T, SIZE>& m2)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			temp[i][j] = m1[i][j] + m2[i][j];
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator+(T scalar, const Matrix<T, SIZE>& m)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			temp[i][j] = scalar + m[i][j];
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator-(const Matrix<T, SIZE>& m, T scalar)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			temp[i][j] = m[i][j] - scalar;
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator-(const Matrix<T, SIZE>& m1, const Matrix<T, SIZE>& m2)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			temp[i][j] = m1[i][j] - m2[i][j];
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator-(T scalar, const Matrix<T, SIZE>& m)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
			temp[i][j] = scalar - m[i][j];
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator*(const Matrix<T, SIZE>& m, T scalar)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			temp[i][j] += m[i][j] * scalar;
		}
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator*(const Matrix<T, SIZE>& m1, const Matrix<T, SIZE>& m2)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			for (size_t k = 0; k < SIZE; k++)
			{
				temp[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}

	return temp;
}

template<typename T, size_t SIZE>
inline Matrix<T, SIZE> operator*(T scalar, const Matrix<T, SIZE>& m)
{
	Matrix<T, SIZE> temp;

	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			temp[i][j] += m[i][j] * scalar;
		}
	}

	return temp;
}

//--------- Matrix3x3 ------------
template<typename T>
inline Vector2<T> operator*(const Matrix<T, 3>& m, const Vector2<T>& v)
{
	Vector2<T> vec;
	vec.x = (m[0][0] * v.x) + (m[0][1] * v.y);
	vec.y = (m[1][0] * v.x) + (m[1][1] * v.y);
	return vec;
}

template<typename T>
inline Vector2<T> operator*(const Vector2<T>& v, const Matrix<T, 3>& m)
{
	Vector2<T> vec;
	vec.x = (v.x * m[0][0]) + (v.y * m[1][0]);
	vec.y = (v.x * m[0][1]) + (v.y * m[1][1]);

	return vec;
}

//--------- Matrix2x2 ------------
template<typename T>
inline Vector2<T> operator*(const Matrix<T, 2>& m, const Vector2<T>& v)
{
	Vector2<T> vec;
	vec.x = (m[0][0] * v.x) + (m[0][1] * v.y);
	vec.y = (m[1][0] * v.x) + (m[1][1] * v.y);
	return vec;
}

template<typename T>
inline Vector2<T> operator*(const Vector2<T>& v, const Matrix<T, 2>& m)
{
	Vector2<T> vec;
	vec.x = (v.x * m[0][0]) + (v.y * m[1][0]);
	vec.y = (v.x * m[0][1]) + (v.y * m[1][1]);

	return vec;
}
// -------- Define Types ----------
// --------------------------------

using Matrix2x2f = Matrix<float, 2>;
using Matrix3x3f = Matrix<float, 3>;
using Matrix4x4f = Matrix<float, 4>;