#include "inside/table.h"

Table::Table() : column_(Column(0.015f, 0.379f, 10, glm::vec3(0.0f,0.0f,0.0f),"../../media/textures/muwen.jpg"))
{
    square_.getVertices() = {
        // --- 位置 ---        --- 法线 ---        --- 颜色 ---        --- 纹理坐标 ---
        // 前面 (z=0.4)
        -0.25f,  0.38f,  0.25f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  0.0f, 0.0f, // 0
         0.25f,  0.38f,  0.25f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  1.0f, 0.0f, // 1
         0.25f,  0.4f,  0.25f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  1.0f, 1.0f, // 2
        -0.25f,  0.4f,  0.25f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  0.0f, 1.0f, // 3

        // 后面 (z=-0.4)
        -0.25f,  0.38f, -0.25f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  1.0f, 0.0f, // 4
         0.25f,  0.38f, -0.25f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  0.0f, 0.0f, // 5
         0.25f,  0.4f, -0.25f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  0.0f, 1.0f, // 6
        -0.25f,  0.4f, -0.25f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  1.0f, 1.0f, // 7

        // 左面 (x=-0.4)
        -0.25f,  0.4f,  0.25f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  1.0f, 0.0f, // 8 (对应前-上-左)
        -0.25f,  0.4f, -0.25f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  1.0f, 1.0f, // 9 (对应后-上-左)
        -0.25f,  0.38f, -0.25f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  0.0f, 1.0f, // 10 (对应后-下-左)
        -0.25f,  0.38f,  0.25f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  0.0f, 0.0f, // 11 (对应前-下-左)

        // 右面 (x=0.4)
         0.25f,  0.4f,  0.25f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  1.0f, 0.0f, // 12 (对应前-上-右)
         0.25f,  0.4f, -0.25f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  1.0f, 1.0f, // 13 (对应后-上-右)
         0.25f,  0.38f, -0.25f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  0.0f, 1.0f, // 14 (对应后-下-右)
         0.25f,  0.38f,  0.25f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  0.0f, 0.0f, // 15 (对应前-下-右)

         // 顶面 (y=0.4)
         -0.25f,  0.4f, -0.25f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  0.0f, 1.0f, // 16 (对应后-上-左)
          0.25f,  0.4f, -0.25f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  1.0f, 1.0f, // 17 (对应后-上-右)
          0.25f,  0.4f,  0.25f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  1.0f, 0.0f, // 18 (对应前-上-右)
         -0.25f,  0.4f,  0.25f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  0.0f, 0.0f, // 19 (对应前-上-左)

         // 底面 (y=-0.4)
         -0.25f,  0.38f, -0.25f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  0.0f, 1.0f, // 20 (对应后-下-左)
          0.25f,  0.38f, -0.25f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  1.0f, 1.0f, // 21 (对应后-下-右)
          0.25f,  0.38f,  0.25f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  1.0f, 0.0f, // 22 (对应前-下-右)
         -0.25f,  0.38f,  0.25f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  0.0f, 0.0f  // 23 (对应前-下-左)
    };

    // 索引数据，定义了组成每个三角形的顶点顺序
    square_.getIndices() = {
        // 前面
        0, 1, 2,
        2, 3, 0,

        // 后面
        4, 5, 6,
        6, 7, 4,

        // 左面
        8, 9, 10,
        10, 11, 8,

        // 右面
        12, 13, 14,
        14, 15, 12,

        // 顶面
        16, 17, 18,
        18, 19, 16,

        // 底面
        20, 21, 22,
        22, 23, 20
    };
    square_.getTexturePath() = "../../media/textures/muwen.jpg";
}

void Table::setup() {
    square_.setup();
    column_.setup();
}

void Table::draw(Shader& shader, const glm::mat4& modelMatrix,bool useTexture) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix, useTexture);
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(0.2f, 0.0f, 0.2f)), useTexture);
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(0.2f, 0.0f, -0.2f)), useTexture);
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(-0.2f, 0.0f, 0.2f)), useTexture);
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(-0.2f, 0.0f, -0.2f)), useTexture);
}