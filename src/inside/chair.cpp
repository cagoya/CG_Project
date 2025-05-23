#include "inside/chair.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// ���κͿ�����ɫ
const float C1_R = 0.35f, C1_G = 0.17f, C1_B = 0.01f;
// ������ɫ
const float C2_R = 0.25f, C2_G = 0.12f, C2_B = 0.005f;

static const float chairVertices[] = {
    // ���� (0-7) - ��ɫ C1
    // ����
    -0.2f, 0.40f, -0.2f,  C1_R, C1_G, C1_B, // 0 �����
     0.2f, 0.40f, -0.2f,  C1_R, C1_G, C1_B, // 1 �Һ���
     0.2f, 0.40f,  0.2f,  C1_R, C1_G, C1_B, // 2 ��ǰ��
    -0.2f, 0.40f,  0.2f,  C1_R, C1_G, C1_B, // 3 ��ǰ��
    // ����
    -0.2f, 0.45f, -0.2f,  C1_R, C1_G, C1_B, // 4 �����
     0.2f, 0.45f, -0.2f,  C1_R, C1_G, C1_B, // 5 �Һ���
     0.2f, 0.45f,  0.2f,  C1_R, C1_G, C1_B, // 6 ��ǰ��
    -0.2f, 0.45f,  0.2f,  C1_R, C1_G, C1_B, // 7 ��ǰ��

    // ���� (8-23) - ��ɫ C2
    // ��ǰ�� (һ����) (8-11)
    -0.18f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 8
    -0.13f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 9
    -0.13f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 10
    -0.18f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 11

    // ��ǰ�� (һ����) (12-15)
     0.13f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 12
     0.18f, 0.0f,  0.18f,  C2_R, C2_G, C2_B, // 13
     0.18f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 14
     0.13f, 0.40f, 0.18f,  C2_R, C2_G, C2_B, // 15

     // ����� (һ����) (16-19)
     -0.18f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 16
     -0.13f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 17
     -0.13f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 18
     -0.18f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 19

     // �Һ��� (һ����) (20-23)
      0.13f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 20
      0.18f, 0.0f, -0.18f,  C2_R, C2_G, C2_B, // 21
      0.18f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 22
      0.13f, 0.40f,-0.18f,  C2_R, C2_G, C2_B, // 23

      // ���� (24-31) - ��ɫ C1
      // "����" (�������δ�, Y=0.45f)
      -0.2f, 0.45f, -0.25f,  C1_R, C1_G, C1_B, // 24 ����� (�����ĺ��µ�)
       0.2f, 0.45f, -0.25f,  C1_R, C1_G, C1_B, // 25 �Һ���
       0.2f, 0.45f, -0.20f,  C1_R, C1_G, C1_B, // 26 ��ǰ�� (������ǰ�µ�, �������κ�Ե)
      -0.2f, 0.45f, -0.20f,  C1_R, C1_G, C1_B, // 27 ��ǰ��
      // "����" (Y=0.90f)
      -0.2f, 0.90f, -0.25f,  C1_R, C1_G, C1_B, // 28 �����
       0.2f, 0.90f, -0.25f,  C1_R, C1_G, C1_B, // 29 �Һ���
       0.2f, 0.90f, -0.20f,  C1_R, C1_G, C1_B, // 30 ��ǰ��
      -0.2f, 0.90f, -0.20f,  C1_R, C1_G, C1_B  // 31 ��ǰ��
};

static unsigned int chairIndices[] = {
    // ���� (���ڶ���0-7)
    0, 1, 2,   0, 2, 3,    // ����
    4, 5, 6,   4, 6, 7,    // ����
    0, 1, 5,   0, 5, 4,    // ���� (Z=-0.2f)
    1, 2, 6,   1, 6, 5,    // ���� (X=0.2f)
    2, 3, 7,   2, 7, 6,    // ǰ�� (Z=0.2f)
    3, 0, 4,   3, 4, 7,    // ���� (X=-0.2f)

    // ���� (ÿ����2��������)
     8,  9, 10,    8, 10, 11,   // ��ǰ��
    12, 13, 14,   12, 14, 15,   // ��ǰ��
    16, 17, 18,   16, 18, 19,   // �����
    20, 21, 22,   20, 22, 23,   // �Һ���

    // ���� (���ڶ���24-31)
    24, 25, 26,  24, 26, 27,   // ���� (Y=0.45f)
    28, 29, 30,  28, 30, 31,   // ���� (Y=0.90f)
    24, 25, 29,  24, 29, 28,   // ���� (Z=-0.25f, �����ı���)
    25, 26, 30,  25, 30, 29,   // ���� (X=0.2f)
    26, 27, 31,  26, 31, 30,   // ǰ�� (Z=-0.20f, �������������ε���)
    27, 24, 28,  27, 28, 31    // ���� (X=-0.2f)
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

    // λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // ���vao_
}

void Chair::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix); 

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, sizeof(chairIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); 
}