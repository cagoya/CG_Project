#include "door.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

// 门的顶点数据（一个竖直长方形，贴在正面墙上）
static const float doorVertices[] = {
    -0.2f, 0.0f, 0.501f,   0.4f, 0.2f, 0.0f,
     0.2f, 0.0f, 0.501f,   0.4f, 0.2f, 0.0f,
     0.2f, 0.3f, 0.501f,   0.4f, 0.2f, 0.0f,
    -0.2f, 0.3f, 0.501f,   0.4f, 0.2f, 0.0f,
};

static const unsigned int doorIndices[] = {
    0, 1, 2,
    0, 2, 3
};

Door::Door() : DrawObject() {}

Door::~Door() {}

void Door::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(doorIndices), doorIndices, GL_STATIC_DRAW);

    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Door::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, sizeof(doorIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
