#version 450 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int IDColor;

in vec4 f_Color;
in vec2 f_TexCoords;
flat in int f_TexIndex;
in flat int f_GameObjectID;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor;
    
    switch(f_TexIndex)
    {
        case 0:
            texColor = texture(u_Textures[0], f_TexCoords);
            break;
        case 1:
            texColor = texture(u_Textures[1], f_TexCoords);
            break;
        case 2:
            texColor = texture(u_Textures[2], f_TexCoords);
            break;
        case 3:
            texColor = texture(u_Textures[3], f_TexCoords);
            break;
        case 4:
            texColor = texture(u_Textures[4], f_TexCoords);
            break;
        case 5:
            texColor = texture(u_Textures[5], f_TexCoords);
            break;
        case 6:
            texColor = texture(u_Textures[6], f_TexCoords);
            break;
        case 7:
            texColor = texture(u_Textures[7], f_TexCoords);
            break;
        case 8:
            texColor = texture(u_Textures[8], f_TexCoords);
            break;
        case 9:
            texColor = texture(u_Textures[9], f_TexCoords);
            break;
        case 10:
            texColor = texture(u_Textures[10], f_TexCoords);
            break;
        case 11:
            texColor = texture(u_Textures[11], f_TexCoords);
            break;
        case 12:
            texColor = texture(u_Textures[12], f_TexCoords);
            break;
        case 13:
            texColor = texture(u_Textures[13], f_TexCoords);
            break;
        case 14:
            texColor = texture(u_Textures[14], f_TexCoords);
            break;
        case 15:
            texColor = texture(u_Textures[15], f_TexCoords);
            break;
        case 16:
            texColor = texture(u_Textures[16], f_TexCoords);
            break;
        case 17:
            texColor = texture(u_Textures[17], f_TexCoords);
            break;
        case 18:
            texColor = texture(u_Textures[18], f_TexCoords);
            break;
        case 19:
            texColor = texture(u_Textures[19], f_TexCoords);
            break;
        case 20:
            texColor = texture(u_Textures[20], f_TexCoords);
            break;
        case 21:
            texColor = texture(u_Textures[21], f_TexCoords);
            break;
        case 22:
            texColor = texture(u_Textures[22], f_TexCoords);
            break;
        case 23:
            texColor = texture(u_Textures[23], f_TexCoords);
            break;
        case 24:
            texColor = texture(u_Textures[24], f_TexCoords);
            break;
        case 25:
            texColor = texture(u_Textures[25], f_TexCoords);
            break;
        case 26:
            texColor = texture(u_Textures[26], f_TexCoords);
            break;
        case 27:
            texColor = texture(u_Textures[27], f_TexCoords);
            break;
        case 28:
            texColor = texture(u_Textures[28], f_TexCoords);
            break;
        case 29:
            texColor = texture(u_Textures[29], f_TexCoords);
            break;
        case 30:
            texColor = texture(u_Textures[30], f_TexCoords);
            break;
        case 31:
            texColor = texture(u_Textures[31], f_TexCoords);
            break;
    }
   
    texColor *= f_Color;

    if(texColor.a < 0.1f)
       discard;

    FragColor = texColor;
    IDColor = f_GameObjectID;
}