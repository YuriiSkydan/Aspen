#version 450 core

uniform vec2 positions[2];
uniform mat3 camera;
uniform mat3 transform;

//const vec2 positions[2] = { vec2(0.0f, 0.0f), vec2(0.5f, 0.0f) };

void main()
{
	vec3 pos = vec3(positions[gl_VertexID], 1.0f) * transform * camera;
	gl_Position = vec4(pos, 1.0f);
}