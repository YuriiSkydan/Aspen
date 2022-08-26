#version 450 core

uniform vec2 positions[2];
uniform mat3 camera;
uniform vec2 position;
uniform float angle;

//const vec2 positions[2] = { vec2(0.0f, 0.0f), vec2(0.5f, 0.0f) };

void main()
{
	float angleInRads = angle * (3.14 / 180.0f);

	float cosValue = cos(angleInRads);
	float sinValue = sin(angleInRads);
	mat2 rotation = mat2( cosValue, sinValue, 
						  -sinValue, cosValue);

	vec2 pos = positions[gl_VertexID] * rotation;
	pos += position;
	
	vec3 newPos = vec3(pos, 1.0f) * camera;

	gl_Position = vec4(newPos, 1.0f);
}