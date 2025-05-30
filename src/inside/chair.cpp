#include "inside/chair.h"

Chair::Chair() : column_(Column(0.01f, 0.239f, 10, glm::vec3(0.0f, 0.0f, 0.0f), "../../media/textures/muwen.jpg"))
{
    square_.getVertices() = {
        // --- 位置 ---        --- 法线 ---        --- 颜色 ---        --- 纹理坐标 ---
        // 前面 (z=0.25)
        -0.10f,  0.24f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  0.0f, 0.0f, // 0
         0.05f,  0.24f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  1.0f, 0.0f, // 1
         0.05f,  0.25f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  1.0f, 1.0f, // 2
        -0.10f,  0.25f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  0.0f, 1.0f, // 3

        // 后面 (z=-0.25)
        -0.10f,  0.24f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  1.0f, 0.0f, // 4
         0.05f,  0.24f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  0.0f, 0.0f, // 5
         0.05f,  0.25f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  0.0f, 1.0f, // 6
        -0.10f,  0.25f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  1.0f, 1.0f, // 7

        // 左面 (x=-0.25)
        -0.10f,  0.25f,  0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  1.0f, 0.0f, // 8 (对应前-上-左)
        -0.10f,  0.25f, -0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  1.0f, 1.0f, // 9 (对应后-上-左)
        -0.10f,  0.24f, -0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  0.0f, 1.0f, // 10 (对应后-下-左)
        -0.10f,  0.24f,  0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  0.0f, 0.0f, // 11 (对应前-下-左)

        // 右面 (x=0.25)
         0.05f,  0.25f,  0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  1.0f, 0.0f, // 12 (对应前-上-右)
         0.05f,  0.25f, -0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  1.0f, 1.0f, // 13 (对应后-上-右)
         0.05f,  0.24f, -0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  0.0f, 1.0f, // 14 (对应后-下-右)
         0.05f,  0.24f,  0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  0.0f, 0.0f, // 15 (对应前-下-右)

         // 顶面 (y=0.25)
         -0.10f,  0.25f, -0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  0.0f, 1.0f, // 16 (对应后-上-左)
          0.05f,  0.25f, -0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  1.0f, 1.0f, // 17 (对应后-上-右)
          0.05f,  0.25f,  0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  1.0f, 0.0f, // 18 (对应前-上-右)
         -0.10f,  0.25f,  0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  0.0f, 0.0f, // 19 (对应前-上-左)

         // 底面 (y=-0.25)
         -0.10f,  0.24f, -0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  0.0f, 1.0f, // 20 (对应后-下-左)
          0.05f,  0.24f, -0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  1.0f, 1.0f, // 21 (对应后-下-右)
          0.05f,  0.24f,  0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  1.0f, 0.0f, // 22 (对应前-下-右)
         -0.10f,  0.24f,  0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  0.0f, 0.0f  // 23 (对应前-下-左)
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

void Chair::setup() {
    square_.setup();
    column_.setup();
}

void Chair::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, glm::translate(modelMatrix, glm::vec3(-0.25f, 0.0f, 0.0f)));
    float angle_degrees = 90.0f;
    float angle_radians = glm::radians(angle_degrees);
    glm::vec3 rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 model = glm::rotate(modelMatrix, angle_radians, rotation_axis);
    square_.draw(shader, glm::translate(model, glm::vec3(0.35f, 0.09f, 0.0f)));
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(-0.22f, 0.0f, 0.08f)));
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(-0.22f, 0.0f, -0.08f)));
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(-0.33f, 0.0f, 0.08f)));
    column_.draw(shader, glm::translate(modelMatrix, glm::vec3(-0.33f, 0.0f, -0.08f)));
}