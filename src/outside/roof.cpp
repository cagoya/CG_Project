#include "roof.h"
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

GableRoof::GableRoof() : DrawObject() {}

GableRoof::~GableRoof() {}

void GableRoof::setup() {
    std::vector<float> vertices = {
        // ��б����Σ�����ԼΪ(-0.8944, 0.4472, 0.0)��
        -0.6f, 0.475f,  0.6f,  -0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f, // 0: ���ǰ
        -0.6f, 0.475f, -0.6f,  -0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f, // 1: ��׺�
         0.0f,  0.625f, -0.6f, -0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f, // 2: �ݼ���
         0.0f,  0.625f,  0.6f, -0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f, // 3: �ݼ�ǰ

         // ��б����Σ�����ԼΪ(0.8944, 0.4472, 0.0)��
          0.6f, 0.475f,  0.6f,   0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f, // 4: �ҵ�ǰ
          0.6f, 0.475f, -0.6f,   0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f, // 5: �ҵ׺�
          0.0f,  0.625f, -0.6f,  0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f, // 6: �ݼ��� (ͬ 2)
          0.0f,  0.625f,  0.6f,  0.8944f, 0.4472f, 0.0f,  0.6f, 0.4f, 0.2f  // 7: �ݼ�ǰ (ͬ 3)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };

    if (vao_ == 0) glGenVertexArrays(1, &vao_);
    if (vbo_ == 0) glGenBuffers(1, &vbo_);
    if (ebo_ == 0) glGenBuffers(1, &ebo_);


    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

void GableRoof::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);

    glBindVertexArray(vao_);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); 

    glBindVertexArray(0);
}