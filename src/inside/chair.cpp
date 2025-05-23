#include "inside/chair.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// 座椅和靠背颜色
const float C1_R = 0.35f, C1_G = 0.17f, C1_B = 0.01f;
// 椅腿颜色
const float C2_R = 0.25f, C2_G = 0.12f, C2_B = 0.005f;

static const float chairVertices[] = {
    // 座椅 (0-7) - 颜色 C1
    // 底面
    -0.2f, 0.40f, -0.2f,  C1_R, C1_G, C1_B, // 0 左后下
     0.2f, 0.40f, -0.2f,  C1_R, C1_G, C1_B, // 1 右后下
     0.2f, 0.40f,  0.2f,  C1_R, C1_G, C1_B, // 2 右前下
    -0.2f, 0.40f,  0.2f,  C1_R, C1_G, C1_B, // 3 左前下
    // 顶面
    -0.2f, 0.45f, -0.2f,  C1_R, C1_G, C1_B, // 4 左后上
     0.2f, 0.45f, -0.2f,  C1_R, C1_G, C1_B, // 5 右后上
     0.2f, 0.45f,  0.2f,  C1_R, C1_G, C1_B, // 6 右前上
    -0.2f, 0.45f,  0.2f,  C1_R, C1_G, C1_B, // 7 左前上

    // 椅腿 (8-23) - 颜色 C2
    // 左前腿 (一个面) (8-11)
    -0.18f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 8
    -0.13f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 9
    -0.13f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 10
    -0.18f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 11

    // 右前腿 (一个面) (12-15)
     0.13f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 12
     0.18f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 13
     0.18f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 14
     0.13f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 15

     // 左后腿 (一个面) (16-19)
     -0.18f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 16
     -0.13f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 17
     -0.13f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 18
     -0.18f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 19

     // 右后腿 (一个面) (20-23)
      0.13f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 20
      0.18f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 21
      0.18f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 22
      0.13f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 23

      // 靠背 (24-31) - 颜色 C1
      // "底面" (连接座椅处, Y=0.45f)
      -0.2f, 0.45f, -0.25f,  C1_R, C1_G, C1_B, // 24 左后下 (靠背的后下点)
       0.2f, 0.45f, -0.25f,  C1_R, C1_G, C1_B, // 25 右后下
       0.2f, 0.45f, -0.20f,  C1_R, C1_G, C1_B, // 26 右前下 (靠背的前下点, 贴近座椅后缘)
      -0.2f, 0.45f, -0.20f,  C1_R, C1_G, C1_B, // 27 左前下
      // "顶面" (Y=0.90f)
      -0.2f, 0.90f, -0.25f,  C1_R, C1_G, C1_B, // 28 左后上
       0.2f, 0.90f, -0.25f,  C1_R, C1_G, C1_B, // 29 右后上
       0.2f, 0.90f, -0.20f,  C1_R, C1_G, C1_B, // 30 右前上
      -0.2f, 0.90f, -0.20f,  C1_R, C1_G, C1_B  // 31 左前上
};

static unsigned int chairIndices[] = {
    // 座椅 (基于顶点0-7)
    0, 1, 2,   0, 2, 3,    // 底面
    4, 5, 6,   4, 6, 7,    // 顶面
    0, 1, 5,   0, 5, 4,    // 后面 (Z=-0.2f)
    1, 2, 6,   1, 6, 5,    // 右面 (X=0.2f)
    2, 3, 7,   2, 7, 6,    // 前面 (Z=0.2f)
    3, 0, 4,   3, 4, 7,    // 左面 (X=-0.2f)

    // 椅腿 (每个腿2个三角形)
     8,  9, 10,    8, 10, 11,   // 左前腿
    12, 13, 14,   12, 14, 15,   // 右前腿
    16, 17, 18,   16, 18, 19,   // 左后腿
    20, 21, 22,   20, 22, 23,   // 右后腿

    // 靠背 (基于顶点24-31)
    24, 25, 26,  24, 26, 27,   // 底面 (Y=0.45f)
    28, 29, 30,  28, 30, 31,   // 顶面 (Y=0.90f)
    24, 25, 29,  24, 29, 28,   // 后面 (Z=-0.25f, 靠背的背面)
    25, 26, 30,  25, 30, 29,   // 右面 (X=0.2f)
    26, 27, 31,  26, 31, 30,   // 前面 (Z=-0.20f, 靠背的面向座椅的面)
    27, 24, 28,  27, 28, 31    // 左面 (X=-0.2f)
};

Chair::Chair() : DrawObject() {}

Chair::~Chair() {}

void Chair::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chairVertices), chairVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(chairIndices), chairIndices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // 解绑vao_
}

void Chair::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix); 

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, sizeof(chairIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); 
}