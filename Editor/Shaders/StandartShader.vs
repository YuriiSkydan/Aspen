/*#version 450 core

layout(location = 0) in vec2 v_Pos;
layout(location = 1) in vec2 v_TexCoords;

out vec2 f_TexCoords;

uniform mat3 camera;
uniform mat3 transform;

uniform bool flipX;
uniform bool flipY;

void main()
{
   //mat3 Tcamera = transpose(camera);
   //mat3 Ttransform = transpose(transform);
   vec3 pos = vec3(v_Pos, 1.0f) * transform * camera;
   gl_Position = vec4(pos, 1.0f);
   //gl_Position = vec4(v_Pos.x, v_Pos.y, 0.0f, 1.0f);


   vec2 texCoords = v_TexCoords;
   if(flipX)
	  texCoords.x *= -1.0f;

   if(flipY)
	  texCoords.y *= -1.0f;
   
   f_TexCoords = texCoords;
}*/
#version 450 core

layout(location = 0) in vec2 v_Pos;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in vec2 v_TexCoords;
layout(location = 4) in float v_GameObjectID;

out vec4 f_Color;
out vec2 f_TexCoords;
out int f_GameObjectID;

uniform mat3 camera;

void main()
{ 
	vec3 pos = vec3(v_Pos, 1.0f) * camera;
	gl_Position = vec4(pos, 1.0f);

	f_Color = v_Color;
	f_TexCoords = v_TexCoords;
	f_GameObjectID = int(v_GameObjectID);
}