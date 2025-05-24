#include "outside/roof.h"
#include <iostream>
#include <vector>
#include<iostream>
#include<stb_image.h>
#include <glm/gtc/type_ptr.hpp>

GableRoof::GableRoof() : DrawObject()
{
    square_.getVertices() = {
        // ��б�����
        -0.6f, 0.475f,  0.6f,  0.0f, 0.0f, // 0: ���ǰ
        -0.6f, 0.475f, -0.6f,  1.0f, 0.0f, // 1: ��׺�
         0.0f,  0.625f, -0.6f,  1.0f, 1.0f, // 2: �ݼ���
         0.0f,  0.625f,  0.6f,  0.0f, 1.0f, // 3: �ݼ�ǰ

         // ��б�����
          0.6f, 0.475f,  0.6f,  0.0f, 0.0f, // 4: �ҵ�ǰ
          0.6f, 0.475f, -0.6f,  1.0f, 0.0f, // 5: �ҵ׺�
          0.0f,  0.625f, -0.6f,  1.0f, 1.0f, // 6: �ݼ��� (ͬ 2)
          0.0f,  0.625f,  0.6f,  0.0f, 1.0f  // 7: �ݼ�ǰ (ͬ 3)
    };

    square_.getIndices() = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };

    square_.getTexturePath() = "../../media/textures/roof.jpg";
}

void GableRoof::setup() {
    square_.setup();
}

void GableRoof::draw(Shader shader, const glm::mat4& modelMatrix) const {
    square_.draw(shader, modelMatrix);
}