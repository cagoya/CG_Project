#include "outside/ground.h"
#include <iostream>

Ground::Ground() : DrawObject(), square_()
{
    square_.getVertices() = {
        // 位置                  // 纹理坐标
        -100.0f, 0.01f, -100.0f,  0.0f, 100.0f,
         100.0f, 0.01f, -100.0f, 100.0f, 100.0f,
         100.0f, 0.01f,  100.0f, 100.0f,  0.0f,
        -100.0f, 0.01f,  100.0f,  0.0f,  0.0f
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
    square_.draw(shader, modelMatrix);
}