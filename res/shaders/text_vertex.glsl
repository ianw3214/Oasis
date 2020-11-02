#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform float u_screenWidth;
uniform float u_screenHeight;

void main()
{
    float x = mix(-1.0, 1.0, vertex.x / u_screenWidth);
    float y = mix(-1.0, 1.0, vertex.y / u_screenHeight);
    gl_Position = vec4(x, y, 0.0, 1.0);
    texCoords = vertex.zw;
}