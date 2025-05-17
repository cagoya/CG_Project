#include "wall.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 这个用于绘制房屋的墙面
static const float WallVertices[] = {
    // 背面
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,

    // 正面
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,

    // 左面
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    // 右面
     0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    // 正面的三角形
     -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
     0.0f, 0.625f, 0.5f, 0.5f, 0.5f, 0.5f,

    // 背面的三角形
     -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
     0.0f, 0.625f, -0.5f, 0.5f, 0.5f, 0.5f,

     // 顶面（Y = 1.0）
	 -0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
	  0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
	  0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
	 -0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f

};

// 立方体的索引数据 (绘制12个三角形)
static const unsigned int WallIndices[] = {
    0, 1, 2,  0, 2, 3,    // 背面
    4, 5, 6,  4, 6, 7,    // 正面
    8, 9,10,  8,10,11,    // 左面
    12,13,14, 12,14,15,   // 右面
    16, 17, 18, //左半屋顶
    19, 20, 21, //右半屋顶
	22, 23, 24,  22, 24, 25 // 顶面

};



Wall::Wall() : DrawObject() {}

Wall::~Wall() {}

void Wall::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(WallVertices), WallVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(WallIndices), WallIndices, GL_STATIC_DRAW);

    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Wall::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);

    glBindVertexArray(vao_);

    glDrawElements(GL_TRIANGLES, sizeof(WallIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}