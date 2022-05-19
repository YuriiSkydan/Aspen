#version 330 core

out vec4 FragColor;
out int color2;

in vec2 f_TexCoords;

uniform vec4 spriteColor;
uniform sampler2D sprite;

void main()
{
   vec4 texColor = texture(sprite, f_TexCoords);
   if(texColor.a < 0.1f)
        discard;

   //FragColor = texColor * spriteColor;

    FragColor = texture2D(sprite, f_TexCoords) * spriteColor;
    color2 = 150;
}
