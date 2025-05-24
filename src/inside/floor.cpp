#include "inside/floor.h"
#include <glad/gl.h>
#include<iostream>
#include<stb_image.h>
#include <glm/gtc/type_ptr.hpp>

Floor::Floor() : DrawObject()
{
    square_.getVertices() = {
        // Î»ÖÃ                  // ÎÆÀí×ø±ê
        -0.5f, 0.02f, -0.5f,  0.0f, 0.5f,
         0.5f, 0.02f, -0.5f,  0.5f, 1.0f,
         0.5f, 0.02f,  0.5f,  0.5f, 0.0f,
        -0.5f, 0.02f,  0.5f,  0.0f, 0.0f
    };

    square_.getIndices() = {
        0, 1, 2,
        0, 2, 3
    };

    square_.getTexturePath() = "../../media/textures/floor.jpg";
}

void Floor::setup() {
    square_.setup();
}

void Floor::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    square_.draw(shader, modelMatrix);
}
