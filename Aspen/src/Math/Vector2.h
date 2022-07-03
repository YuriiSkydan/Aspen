#pragma once
#include <cmath>

#include "../Core/Core.h"

#ifdef VECTOR2F

struct ASPEN Vector2f
{
	float x;
	float y;

	Vector2f();
	Vector2f(const float* values);
	Vector2f(float x, float y);

	Vector2f(const Vector2f& copy) = default;

	const Vector2f& operator=(const Vector2f& v);

	const Vector2f& operator=(float value);

	const Vector2f& operator=(float* arr);

	void operator+=(float scalar);
	void operator+=(const Vector2f& v);

	void operator-=(float scalar);
	void operator-=(const Vector2f& v);

	void operator*=(float scalar);

	Vector2f Normalized();

	void Normalize();
	float Magnitude();
};

Vector2f::Vector2f()
	:x(0), y(0)
{
}

Vector2f::Vector2f(const float* values)
	: x(values[0]), y(values[1])
{
}

Vector2f::Vector2f(float x, float y)
	: x(x), y(y)
{
}

const Vector2f& Vector2f::operator=(float* arr)
{
	x = arr[0];
	y = arr[1];
	return *this;
}


const Vector2f& Vector2f::operator=(const Vector2f& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

const Vector2f& Vector2f::operator=(float value)
{
	x = value;
	y = value;
	return *this;
}

void Vector2f::operator+=(float scalar)
{
	x += scalar;
	y += scalar;
}

void Vector2f::operator+=(const Vector2f& v)
{
	x += v.x;
	y += v.y;
}

void Vector2f::operator-=(float scalar)
{
	x -= scalar;
	y -= scalar;
}

inline void Vector2f::operator-=(const Vector2f& v)
{
	x -= v.x;
	y -= v.y;
}

inline void Vector2f::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
}

Vector2f Vector2f::Normalized()
{
	float magnitude = Magnitude();
	return Vector2f(x / magnitude, y / magnitude);
}

void Vector2f::Normalize()
{
	float magnitude = Magnitude();

	if (magnitude == 0)
		return;

	x /= magnitude;
	y /= magnitude;
}

float Vector2f::Magnitude()
{
	return sqrt(x * x + y * y);
}

//--------------------------------
// -- Operations --
float Dot(const Vector2f& v1, const Vector2f& v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y));
}

float Cross(const Vector2f& v1, const Vector2f& v2)
{
	return ((v1.x * v2.y) - (v1.y * v2.x));
}

Vector2f Cross(const Vector2f& v, float s)
{
	return Vector2f(s * v.y, -s * v.x);
}

Vector2f Cross(float s, const Vector2f& v)
{
	return Vector2f(s * v.y, -s * v.x);
}

//--------------------------------
// -- Unary operators --
Vector2f operator+(const Vector2f& v)
{
	return v;
}

template<typename T>
Vector2f operator-(const Vector2f& v)
{
	return Vector2f(-v.x, -v.y);
}


// -- Binary operators --
Vector2f operator+(const Vector2f& v, float scalar)
{
	return Vector2f(v.x + scalar, v.y + scalar);
}

Vector2f operator+(const Vector2f& v1, const Vector2f& v2)
{
	return Vector2f(v1.x + v2.x, v1.y + v2.y);
}

Vector2f operator+(float scalar, const Vector2f& v)
{
	return Vector2f(scalar + v.x, scalar + v.y);
}

Vector2f operator-(const Vector2f& v, float scalar)
{
	return Vector2f(v.x - scalar, v.y - scalar);
}

Vector2f operator-(const Vector2f& v1, const Vector2f& v2)
{
	return Vector2f(v1.x - v2.x, v1.y - v2.y);
}

Vector2f operator-(float scalar, const Vector2f& v)
{
	return Vector2f(scalar + v.x, scalar + v.y);
}

Vector2f operator*(const Vector2f& v, float scalar)
{
	return Vector2f(v.x * scalar, v.y * scalar);
}

float operator*(const Vector2f& v1, const Vector2f& v2)
{
	return (v1.x + v2.x) + (v1.y * v2.y);
}

Vector2f operator*(float scalar, const Vector2f& v)
{
	return Vector2f(v.x * scalar, v.y * scalar);
}

Vector2f operator/(const Vector2f& v, float scalar)
{
	return Vector2f(v.x / scalar, v.y / scalar);
}

// -- Boolean operators --
bool operator==(const Vector2f& v1, const Vector2f& v2)
{
	return ((v1.x == v2.x) && (v1.y == v1.y));
}

bool operator!=(const Vector2f& v1, const Vector2f& v2)
{
	return ((v1.x != v2.x) || (v1.y != v2.y));
}
#endif

#define VECTOR2
#ifdef VECTOR2
#pragma region Vector2
template<typename T = float>
struct Vector2
{
	T x, y;

	Vector2();
	Vector2(const T* values);
	Vector2(T x, T y);

	Vector2(const Vector2<T>& copy) = default;

	template<typename U>
	const Vector2<T>& operator=(const Vector2<U>& v);

	template<typename U>
	const Vector2<T>& operator=(U value);

	const Vector2<T>& operator=(T* arr);

	template<typename U>
	void operator+=(U scalar);
	template<typename U>
	void operator+=(const Vector2<U>& v);

	template<typename U>
	void operator-=(U scalar);
	template<typename U>
	void operator-=(const Vector2<U>& v);

	template<typename U>
	void operator*=(U scalar);

	Vector2<T> Normalized();

	void Normalize();
	float Magnitude();
};


template<typename T>
inline Vector2<T>::Vector2()
	:x(0), y(0)
{
}

template<typename T>
inline Vector2<T>::Vector2(const T* values)
	: x(values[0]), y(values[1])
{
}

template<typename T>
inline Vector2<T>::Vector2(T x, T y)
	: x(x), y(y)
{
}

template<typename T>
inline const Vector2<T>& Vector2<T>::operator=(T* arr)
{
	x = arr[0];
	y = arr[1];
	return *this;
}

template<typename T>
template<typename U>
inline const Vector2<T>& Vector2<T>::operator=(const Vector2<U>& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template<typename T>
template<typename U>
inline const Vector2<T>& Vector2<T>::operator=(U value)
{
	x = value;
	y = value;
	return *this;
}

template<typename T>
template<typename U>
inline void Vector2<T>::operator+=(U scalar)
{
	x += scalar;
	y += scalar;
}

template<typename T>
template<typename U>
inline void Vector2<T>::operator+=(const Vector2<U>& v)
{
	x += v.x;
	y += v.y;
}

template<typename T>
template<typename U>
inline void Vector2<T>::operator-=(U scalar)
{
	x -= scalar;
	y -= scalar;
}

template<typename T>
template<typename U>
inline void Vector2<T>::operator-=(const Vector2<U>& v)
{
	x -= v.x;
	y -= v.y;
}

template<typename T>
template<typename U>
inline void Vector2<T>::operator*=(U scalar)
{
	x *= scalar;
	y *= scalar;
}

template<typename T>
inline Vector2<T> Vector2<T>::Normalized()
{
	float magnitude = Magnitude();
	return Vector2(x / magnitude, y / magnitude);
}

template<typename T>
inline void Vector2<T>::Normalize()
{
	float magnitude = Magnitude();

	if (magnitude == 0)
		return;

	x /= magnitude;
	y /= magnitude;
}

template<typename T>
inline float Vector2<T>::Magnitude()
{
	return sqrt(x * x + y * y);
}

//--------------------------------
// -- Operations --
template<typename T>
T Dot(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y));
}

template<typename T>
T Cross(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return ((v1.x * v2.y) - (v1.y * v2.x));
}

template<typename T>
Vector2<T> Cross(const Vector2<T>& v, float s)
{
	return Vector2<T>(s * v.y, -s * v.x);
}

template<typename T>
Vector2<T> Cross(float s, const Vector2<T>& v)
{
	return Vector2<T>(s * v.y, -s * v.x);
}

//--------------------------------
// -- Unary operators --
template<typename T>
Vector2<T> operator+(const Vector2<T>& v)
{
	return v;
}

template<typename T>
Vector2<T> operator-(const Vector2<T>& v)
{
	return Vector2<T>(-v.x, -v.y);
}


// -- Binary operators --
template<typename T>
inline Vector2<T> operator+(const Vector2<T>& v, T scalar)
{
	return Vector2<T>(v.x + scalar, v.y + scalar);
}

template<typename T>
inline Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return Vector2<T>(v1.x + v2.x, v1.y + v2.y);
}

template<typename T>
inline Vector2<T> operator+(T scalar, const Vector2<T>& v)
{
	return Vector2<T>(scalar + v.x, scalar + v.y);
}

template<typename T>
inline Vector2<T> operator-(const Vector2<T>& v, T scalar)
{
	return Vector2<T>(v.x - scalar, v.y - scalar);
}

template<typename T>
inline Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return Vector2<T>(v1.x - v2.x, v1.y - v2.y);
}

template<typename T>
inline Vector2<T> operator-(T scalar, const Vector2<T>& v)
{
	return Vector2<T>(scalar + v.x, scalar + v.y);
}

template<typename T>
inline Vector2<T> operator*(const Vector2<T>& v, T scalar)
{
	return Vector2<T>(v.x * scalar, v.y * scalar);
}

template<typename T>
inline float operator*(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return (v1.x + v2.x) + (v1.y * v2.y);
}

template<typename T>
inline Vector2<T> operator*(T scalar, const Vector2<T>& v)
{
	return Vector2<T>(v.x * scalar, v.y * scalar);
}

template<typename T>
inline Vector2<T> operator/(const Vector2<T>& v, T scalar)
{
	return Vector2<T>(v.x / scalar, v.y / scalar);
}

// -- Boolean operators --
template<typename T>
inline bool operator==(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return ((v1.x == v2.x) && (v1.y == v1.y));
}

template<typename T>
inline bool operator!=(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return ((v1.x != v2.x) || (v1.y != v2.y));
}

// -- Define Types --
using Vector2ui = Vector2<unsigned int>;
using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

#pragma endregion
#endif