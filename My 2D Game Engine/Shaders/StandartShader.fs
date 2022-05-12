#version 330 core

out vec4 FragColor;

in vec2 f_TexCoords;

uniform vec4 color;
uniform sampler2D sprite;

void main()
{
   // vec4 texColor = texture(sprite, f_TexCoords);
    //if(texColor.a < 0.1f)
      //  discard;
    FragColor =/* texture2D(sprite, f_TexCoords) */ color;
}
