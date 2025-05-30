#include "outside/ground.h"
#include <iostream>

Ground::Ground() : DrawObject(), square_()
{
    square_.getVertices() = {
        // First vertex
        -100.0f, 0.01f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 100.0f,
        // Second vertex
    	100.0f, 0.01f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 100.0f, 100.0f,
    	// Third vertex
    	100.0f, 0.01f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 100.0f,  0.0f,
    	// Fourth vertex
    	-100.0f, 0.01f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f
    };

    square_.getIndices() = {
        0, 1, 3,
        1, 2, 3
    };

    square_.getTexturePath() = "../../media/textures/grass.jpg";
}

// 绑定 vao_ vbo_ ebo_ 
void Ground::setup() {
    square_.setup();
}

// 绘制地面
void Ground::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
}