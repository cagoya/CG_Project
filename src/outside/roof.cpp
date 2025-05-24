#include "outside/roof.h"
#include <iostream>
#include <vector>
#include<iostream>
#include<stb_image.h>
#include <glm/gtc/type_ptr.hpp>

GableRoof::GableRoof() : DrawObject()
{
    square_.getVertices() = {
        // ×óÐ±Ãæ (Left Oblique Face)
		// 0: ×óµ×Ç°
		-0.6f, 0.475f,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		// 1: ×óµ×ºó
		-0.6f, 0.475f, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		// 2: ¶¥¼¹ºó
    	0.0f, 0.625f, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    	// 3: ¶¥¼¹Ç°
    	0.0f, 0.625f,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    	// ÓÒÐ±Ãæ (Right Oblique Face)
    	// 4: ÓÒµ×Ç°
    	0.6f, 0.475f,  0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    	// 5: ÓÒµ×ºó
    	0.6f, 0.475f, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    	// 6: ¶¥¼¹ºó (Í¬ 2)
    	0.0f, 0.625f, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    	// 7: ¶¥¼¹Ç° (Í¬ 3)
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