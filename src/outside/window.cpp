#include "Window.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

// 窗户顶点：贴在右侧墙面，居中
static const float windowVertices[] = {
    // 位置                // 法线         // 颜色
     0.501f, 0.175f, -0.15f,  1.0f, 0.0f, 0.0f,  0.4f, 0.7f, 1.0f, // 左下
     0.501f, 0.175f,  0.15f,  1.0f, 0.0f, 0.0f,  0.4f, 0.7f, 1.0f, // 右下
     0.501f, 0.325f,  0.15f,  1.0f, 0.0f, 0.0f,  0.4f, 0.7f, 1.0f, // 右上
     0.501f, 0.325f, -0.15f,  1.0f, 0.0f, 0.0f,  0.4f, 0.7f, 1.0f, // 左上
};

static const unsigned int windowIndices[] = {
    0, 1, 2,
    0, 2, 3
};

Window::Window() : DrawObject() {}

Window::~Window() {}

void Window::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowIndices), windowIndices, GL_STATIC_DRAW);

    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 法线
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 颜色
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);
}

void Window::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
