#include "outside/door.h"
#include <glad/gl.h>
#include<stb_image.h>
#include<iostream>
#include <glm/gtc/type_ptr.hpp>

Door::Door() : DrawObject(),square_()
{
    square_.getVertices() = {
        -0.1f, 0.0f, 0.501f,   0.0f, 0.0f, // 左下
	     0.1f, 0.0f, 0.501f,   1.0f, 0.0f, // 右下
	     0.1f, 0.3f, 0.501f,   1.0f, 1.0f, // 右上
	    -0.1f, 0.3f, 0.501f,   0.0f, 1.0f  // 左上
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
    square_.draw(shader, modelMatrix);
}
