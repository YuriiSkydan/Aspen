#pragma once
#include "Matrix.h"
//#include <numbers>
#include <iostream> // delete it

const float PI = 3.14159265359f;

namespace MatrixTransform
{
	template<typename T>
	Matrix<T, 3> Translate(const Matrix<T, 3>& m, const Vector2<T>& position)
	{
		Matrix<T, 3> translated = m;
		translated[0][2] = m[0][0] * position.x + m[0][1] * position.y + m[0][2];
		translated[1][2] = m[1][0] * position.x + m[1][1] * position.y + m[1][2];

		return translated;
	}

	template<typename T>
	Matrix<T, 3> Rotate(const Matrix<T, 3>& matrix, float angle)
	{
		Matrix<T, 2> rotation(1.0f);

		float cosValue = cos(angle * (PI / 180.0f));
		float sinValue = sin(angle * (PI / 180.0f));

		rotation[0][0] = cosValue;
		rotation[0][1] = sinValue;

		rotation[1][0] = -sinValue;
		rotation[1][1] = cosValue;

		Matrix<T, 3> result = matrix;
		result = rotation * Matrix<T, 2>(matrix);

		return result;
	}

	template<typename T>
	Matrix<T, 3> Scale(const Matrix<T, 3>& matrix, const Vector2<T>& vector)
	{
		Matrix<T, 2> scaled;
		scaled[0][0] = vector.x;
		scaled[1][1] = vector.y;

		Matrix<T, 3> result = matrix;
		result = scaled * Matrix<T, 2>(matrix);

		return result;
	}

	template<typename T>
	Matrix<T, 3> CameraMatrix(Vector2<T> position, float ratio)
	{
		Matrix<T, 3> cameraMatrix = Matrix<T, 3>(1.0f);

		cameraMatrix = Translate(cameraMatrix, position);
		cameraMatrix[1][1] = ratio;

		return cameraMatrix;
	}
}