#include "outside/Window.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "stb_image.h"

Window::Window() : DrawObject()
{
    square_.getVertices() = {
        // 位置 (Position)             // 法向量 (Normal)     // 颜色 (Color)         // 纹理坐标 (Texture)
        // 左下 (Bottom-left)
        0.501f, 0.175f, -0.15f,        1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // 右下 (Bottom-right)
        0.501f, 0.175f,  0.15f,        1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // 右上 (Top-right)
        0.501f, 0.325f,  0.15f,        1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // 左上 (Top-left)
        0.501f, 0.325f, -0.15f,        1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 1.0f
    };

    square_.getIndices() = {
        0, 1, 3,
        1, 2, 3
    };

    square_.getTexturePath() = "../../media/textures/window.png";
}

void Window::setup() {
    square_.setup();
}

void Window::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    square_.draw(shader, modelMatrix);
    glDisable(GL_BLEND);
}
