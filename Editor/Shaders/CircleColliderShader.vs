#version 450 core

//uniform mat3 camera;
//uniform float radius;

const vec2 quadVerticies[4] = { vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f), vec2(-0.5f, 0.5f), vec2(0.5f, 0.5f) };
//const vec2 quadVerticies[4] = { vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f)};

void main()
{
   gl_Position = vec4(quadVerticies[gl_VertexID], 0.0f, 1.0f);
}