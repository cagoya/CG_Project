#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos_worldspace;
out vec3 Normal_worldspace;
out vec2 TexCoords_FS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    FragPos_worldspace = vec3(model * vec4(aPos, 1.0));

    Normal_worldspace = mat3(transpose(inverse(model))) * aNormal;

    TexCoords_FS = aTexCoords;
}