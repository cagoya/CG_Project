#include "Ground.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

// һ����������Ϊ���棬��ɫΪ��ɫ��֮����ͼΪ�ݵ�
static const float vertices[] = {
     1.0f, 0.0f,  1.0f,  0.0f, 0.3f, 0.0f,
     1.0f, 0.0f, -1.0f,  0.0f, 0.3f, 0.0f,
    -1.0f, 0.0f, -1.0f,  0.0f, 0.3f, 0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f, 0.3f, 0.0f
};

static const unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};


Ground::Ground() : DrawObject() {}

Ground::~Ground() {}

// �� vao_ vbo_ ebo_ 
void Ground::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// ���Ƶ���
void Ground::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}