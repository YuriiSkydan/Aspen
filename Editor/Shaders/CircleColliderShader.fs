#version 450 core

layout (location = 0) out vec4 FragColor;

void main()
{
   float x = gl_FragCoord.x / (1920.0f / 2.0f) - 1.0f;
   float y = gl_FragCoord.y / (1080.0f / 2.0f) - 1.0f;

   vec2 to = vec2(x, y) - vec2(0.0f, 0.0f);
   
   float length = sqrt(to.x * to.x + to.y * to.y);
   
   if(length > 0.5f)
       discard;

   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}