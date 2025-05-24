#include "table.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

// ���㣺���� + �ĸ����ȣ�ÿ��������ɫ����Ϊ����ɫ
static const float tableVertices[] = {

    // ���涥�� (0,1,0)
    -0.5f, 0.25f, -0.3f,  0.0f, 1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
     0.5f, 0.25f, -0.3f,  0.0f, 1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
     0.5f, 0.25f,  0.3f,  0.0f, 1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
    -0.5f, 0.25f,  0.3f,  0.0f, 1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
    // ������� (0,-1,0)
    -0.5f, 0.20f, -0.3f,  0.0f, -1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
     0.5f, 0.20f, -0.3f,  0.0f, -1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
     0.5f, 0.20f,  0.3f,  0.0f, -1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
    -0.5f, 0.20f,  0.3f,  0.0f, -1.0f, 0.0f,  0.4f, 0.2f, 0.0f,
    // ���ȣ�����ǰ��Ϊ�������߿ɼ�Ϊ(0,0,1)��ʵ���淽����д��
    -0.48f, 0.0f,  0.28f,  0.0f, 0.0f, 1.0f,  0.3f, 0.15f, 0.0f,
    -0.43f, 0.0f,  0.28f,  0.0f, 0.0f, 1.0f,  0.3f, 0.15f, 0.0f,
    -0.43f, 0.25f, 0.28f,  0.0f, 0.0f, 1.0f,  0.3f, 0.15f, 0.0f,
    -0.48f, 0.25f, 0.28f,  0.0f, 0.0f, 1.0f,  0.3f, 0.15f, 0.0f,

    // ��ǰ
     0.43f, 0.0f,  0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
     0.48f, 0.0f,  0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
     0.48f, 0.25f, 0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
     0.43f, 0.25f, 0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,

     // ���
     -0.48f, 0.0f, -0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
     -0.43f, 0.0f, -0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
     -0.43f, 0.25f, -0.28f,0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
     -0.48f, 0.25f, -0.28f,0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,

     // �Һ�
      0.43f, 0.0f, -0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
      0.48f, 0.0f, -0.28f, 0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
      0.48f, 0.25f, -0.28f,0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
      0.43f, 0.25f, -0.28f,0.0f, 0.0f, 1.0f, 0.3f, 0.15f, 0.0f,
};

static unsigned int tableIndices[] = {
    // ���� top & bottom
    0, 1, 2, 0, 2, 3,
    4, 5, 6, 4, 6, 7,
    // ������
    0, 1, 5, 0, 5, 4,
    1, 2, 6, 1, 6, 5,
    2, 3, 7, 2, 7, 6,
    3, 0, 4, 3, 4, 7,
    // ÿ�����Ȼ���һȦ
    8, 9,10, 8,10,11,
    12,13,14,12,14,15,
    16,17,18,16,18,19,
    20,21,22,20,22,23
};

Table::Table() : VAO(0), VBO(0), EBO(0) {}

Table::~Table() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void Table::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tableIndices), tableIndices, GL_STATIC_DRAW);

    // λ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ��ɫ
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Table::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(tableIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}
