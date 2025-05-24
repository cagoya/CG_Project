#include "outside/roof.h"
#include <iostream>
#include <vector>
#include<iostream>
#include<stb_image.h>
#include <glm/gtc/type_ptr.hpp>

GableRoof::GableRoof() : DrawObject()
{
    square_.getVertices() = {
        // ��б�� (Left Oblique Face)
		// 0: ���ǰ
		-0.6f, 0.475f,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		// 1: ��׺�
		-0.6f, 0.475f, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		// 2: ������
    	0.0f, 0.625f, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    	// 3: ����ǰ
    	0.0f, 0.625f,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    	// ��б�� (Right Oblique Face)
    	// 4: �ҵ�ǰ
    	0.6f, 0.475f,  0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    	// 5: �ҵ׺�
    	0.6f, 0.475f, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    	// 6: ������ (ͬ 2)
    	0.0f, 0.625f, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    	// 7: ����ǰ (ͬ 3)
    	0.0f, 0.625f,  0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
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
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
}