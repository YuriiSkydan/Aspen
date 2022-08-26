#version 450 core

layout(location = 0) in vec2 v_VertexPos;
layout(location = 1) in vec2 v_QuadPos;
layout(location = 2) in vec2 v_Scale;
layout(location = 3) in float v_Angle;
layout(location = 4) in vec4 v_Color;
layout(location = 5) in vec2 v_TexCoords;
layout(location = 6) in float v_TexIndex;
layout(location = 7) in float v_GameObjectID;

out vec4 f_Color;
out vec2 f_TexCoords;
flat out int f_TexIndex;
out int f_GameObjectID;

uniform mat3 camera;

void main()
{ 
	float angle = v_Angle * (3.14f / 180.0f);

	mat2 scaling = mat2( v_Scale.x, 0.0f,
						 0.0f, v_Scale.y);

	float cosValue = cos(angle);
	float sinValue = sin(angle);
	mat2 rotation = mat2( cosValue, sinValue,
	                     -sinValue, cosValue);

	vec2 newPos = v_VertexPos * scaling * rotation;
	newPos += v_QuadPos;

	vec3 pos = vec3(newPos, 1.0f) * camera;
	gl_Position = vec4(pos, 1.0f);

	f_Color = v_Color;
	f_TexCoords = v_TexCoords;
	f_TexIndex = int(v_TexIndex);
	f_GameObjectID = int(v_GameObjectID);
}