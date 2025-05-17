#include "wall.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

// ������ڻ��Ʒ��ݵ�ǽ��
static const float WallVertices[] = {
    // ����
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,

    // ����
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,

    // ����
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    // ����
     0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    // �����������
     -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
     0.0f, 0.625f, 0.5f, 0.5f, 0.5f, 0.5f,

    // �����������
     -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
     0.0f, 0.625f, -0.5f, 0.5f, 0.5f, 0.5f,

     // ���棨Y = 1.0��
	 -0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
	  0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
	  0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
	 -0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f

};

// ��������������� (����12��������)
static const unsigned int WallIndices[] = {
    0, 1, 2,  0, 2, 3,    // ����
    4, 5, 6,  4, 6, 7,    // ����
    8, 9,10,  8,10,11,    // ����
    12,13,14, 12,14,15,   // ����
    16, 17, 18, //����ݶ�
    19, 20, 21, //�Ұ��ݶ�
	22, 23, 24,  22, 24, 25 // ����

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

    // λ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ��ɫ
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