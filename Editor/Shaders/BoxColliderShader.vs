#version 450 core

layout(location = 0) in vec2 v_Pos;

uniform mat3 camera;
uniform mat3 transform;

void main()
{
   vec3 pos = vec3(v_Pos, 1.0f) * transform * camera;
   gl_Position = vec4(pos, 1.0f);
}