#pragma once
#include "Matrix.h"

template<typename T>
Matrix<T, 3> Translate(const Matrix<T, 3>& matrix, const Vector2<T>& position)
{
	Matrix<T, 3> translated = matrix;
	translated[0][2] = position.x;
	translated[1][2] = position.y;

	return translated;
}

template<typename T>
Matrix<T, 3> Rotate(const Matrix<T, 3>& matrix, float angle)
{
	Matrix<T, 3> rotation(1.0f);

	float cosValue = cos(angle * (std::numbers::pi / 180));
	float sinValue = sin(angle * (std::numbers::pi / 180));

	rotation[0][0] = cosValue;
	rotation[0][1] = sinValue;

	rotation[1][0] = -sinValue;
	rotation[1][1] = cosValue;

	return matrix * rotation;
}

template<typename T>
Matrix<T, 3> Scale(const Matrix<T, 3>& matrix, const Vector2<T>& vector)
{
	Matrix<T, 3> scaled(1.0f);
	scaled[0][0] *= vector.x;
	scaled[1][0] *= vector.x;

	scaled[0][1] *= vector.y;
	scaled[1][1] *= vector.y;

	return matrix * scaled;
}

template<typename T>
Matrix<T, 3> CameraMatrix(Vector2<T> position, float ratio)
{
	Matrix<T, 3> cameraMatrix = Matrix<T, 3>(1.0f);

	cameraMatrix = Translate(cameraMatrix, position);
	cameraMatrix[0][0] = ratio;

	return cameraMatrix;
}