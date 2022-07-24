#version 450 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int IDColor;

in vec2 f_TexCoords;

uniform int gameObjectID; 
uniform vec4 spriteColor;
uniform sampler2D sprite;

void main()
{
   vec4 texColor = texture(sprite, f_TexCoords) * spriteColor;
  
   if(texColor.a < 0.1f)
        discard;

   FragColor = texColor;
  // FragColor = spriteColor;
   IDColor = gameObjectID;
}
