#include "EditorCamera.h"
#include "../Input/Input.h"

#include <iostream>

EditorCamera::EditorCamera(float ratio)
	: m_Ratio(ratio), m_Scale(1.0f)
{
}

void EditorCamera::Update()
{
	if (Input::IsMouseButtonPressed(Mouse::Button3) || Input::IsKeyPressed(Key::Up))
		m_Scale += 0.01f;
	else if ((Input::IsMouseButtonPressed(Mouse::Button4) || Input::IsKeyPressed(Key::Down)) && m_Scale > 0.01f)
		m_Scale -= 0.01f;

	static Vector2 startPosition;
	if (Input::IsMouseButtonPressed(Mouse::Button1))
	{
		if (startPosition == Vector2f(0, 0))
			startPosition = Input::GetMousePosition();

		Vector2 moved;
		moved += Input::GetMousePosition() - startPosition;

		m_Position.x -= moved.x * 0.001f;
		m_Position.y += moved.y * 0.001f;
		startPosition = Input::GetMousePosition();
	}
	else
	{
		startPosition = Vector2f(0, 0);
	}
}

Matrix3x3f EditorCamera::GetCameraMatrix() const
{
	Matrix3x3f cameraMatrix = MatrixTransform::CameraMatrix(-m_Position, m_Ratio);
	cameraMatrix[0][0] *= m_Scale;
	cameraMatrix[1][1] *= m_Scale;
	return cameraMatrix;
}
