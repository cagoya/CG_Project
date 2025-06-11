#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform float time;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    vec3 pos = aPos;

    // --- 波浪 ---
    float amplitude = 0.9f;
    float frequency = 5.0f;
    float speed = 2.5f;

    pos.y += amplitude * sin(pos.x * frequency + time * speed);
    pos.y += amplitude * cos(pos.z * frequency * 0.7 + time * speed * 1.2);

    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = normalize(normalMatrix * aNormal);
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}