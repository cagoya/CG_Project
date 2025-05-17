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
    // 天空盒图片的路径
    Skybox(const std::vector<std::string>& faces);
    // 绘制天空盒
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int vao_, vbo_;
    unsigned int textureID_;
    // 读取天空盒的六个面
    unsigned int loadCubemap(const std::vector<std::string>& faces);
    void setupSkybox();
};

#endif
