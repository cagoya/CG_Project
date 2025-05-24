#include "outside/Window.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "stb_image.h"

Window::Window() : DrawObject()
{
    square_.getVertices() = {
        // 位置              // 纹理坐标
	    0.501f, 0.175f, -0.15f,   0.0f, 0.0f, // 左下
	    0.501f, 0.175f,  0.15f,   1.0f, 0.0f, // 右下
	    0.501f, 0.325f,  0.15f,   1.0f, 1.0f, // 右上
	    0.501f, 0.325f, -0.15f,   0.0f, 1.0f  // 左上
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    square_.draw(shader, modelMatrix);
    glDisable(GL_BLEND);
}
