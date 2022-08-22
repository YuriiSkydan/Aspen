#version 450 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out int IDColor;

in vec4 f_Color;
in vec2 f_TexCoords;
in float f_TexIndex;
in flat int f_GameObjectID;

uniform sampler2D u_Textures[32];

void main()
{
    int index = int(f_TexIndex);
    vec4 texColor = texture(u_Textures[index], f_TexCoords) * f_Color;
   
    if(texColor.a < 0.1f)
        discard;

    FragColor = texColor;
    IDColor = f_GameObjectID;
}