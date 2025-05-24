#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "shader.h"

class Skybox
{
public:
    // ��պ�ͼƬ��·��
    Skybox(const std::vector<std::string>& faces);
    // ������պ�
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int vao_, vbo_;
    unsigned int textureID_;
    // ��ȡ��պе�������
    unsigned int loadCubemap(const std::vector<std::string>& faces);
    void setupSkybox();
};

#endif
