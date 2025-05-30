#include "outside/roof.h"
#include <iostream>
#include <vector>
#include<iostream>
#include<stb_image.h>
#include <glm/gtc/type_ptr.hpp>

GableRoof::GableRoof() : DrawObject()
{
    square_.getVertices() = {
        // 左斜面 (Left Oblique Face)
		// 0: 左底前
		-0.6f, 0.475f,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		// 1: 左底后
		-0.6f, 0.475f, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		// 2: 顶脊后
    	0.0f, 0.625f, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    	// 3: 顶脊前
    	0.0f, 0.625f,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    	// 右斜面 (Right Oblique Face)
    	// 4: 右底前
    	0.6f, 0.475f,  0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    	// 5: 右底后
    	0.6f, 0.475f, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    	// 6: 顶脊后 (同 2)
    	0.0f, 0.625f, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    	// 7: 顶脊前 (同 3)
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