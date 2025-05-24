#include "outside/door.h"
#include <glad/gl.h>
#include<stb_image.h>
#include<iostream>
#include <glm/gtc/type_ptr.hpp>

Door::Door() : DrawObject(),square_()
{
    square_.getVertices() = {
        // ���� (Bottom-left)
		-0.1f, 0.0f, 0.501f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		// ���� (Bottom-right)
		 0.1f, 0.0f, 0.501f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    	// ���� (Top-right)
    	0.1f, 0.3f, 0.501f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    	// ���� (Top-left)
    	-0.1f, 0.3f, 0.501f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };

    square_.getIndices() = {
        0, 1, 2,
		0, 2, 3
    };

    square_.getTexturePath() = "../../media/textures/door.png";
}

void Door::setup() {
    square_.setup();
}

void Door::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
}
