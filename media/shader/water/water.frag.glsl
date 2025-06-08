#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
    // 简单的蓝色水面
    FragColor = vec4(0.0, 0.5, 0.8, 0.8);
} 