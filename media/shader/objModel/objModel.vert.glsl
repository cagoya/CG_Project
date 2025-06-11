#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

// 添加光照相关的uniform
uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;
uniform vec3 directionalLightDirection;
uniform vec3 directionalLightColor;
uniform float directionalLightIntensity;
uniform vec3 spotLightPosition;
uniform vec3 spotLightDirection;
uniform vec3 spotLightColor;
uniform float spotLightIntensity;
uniform float spotLightAngle;
uniform float spotLightKc;
uniform float spotLightKl;
uniform float spotLightKq;

void main()
{
    vec2 waterTexCoords = aTexCoords;
    if (aNormal.y > 0.9) {
        waterTexCoords.x += time * 0.1;
        waterTexCoords.y += time * 0.05;
        if (gl_VertexID == 0) {
        }
    }
    TexCoords = waterTexCoords;
    
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    TBN = mat3(T, B, N);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}