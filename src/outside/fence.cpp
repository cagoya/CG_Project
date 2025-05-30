#include "outside/fence.h"

Fence::Fence()
{
    float thickness = 0.05f; // 围栏的厚度

    square_.getVertices() = {
        // 前面 (Front Face)
        // 位置 (Position)                   // 法向量 (Normal)     // 颜色 (Color)         // 纹理坐标 (Texture)
        // 左下 (Bottom-left)
        -0.5f, 0.03f, -3.5f + thickness / 2,   0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        // 右下 (Bottom-right)
        0.5f, 0.03f, -3.5f + thickness / 2,    0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        // 右上 (Top-right)
        0.5f, 0.5f,  -3.5f + thickness / 2,    0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        // 左上 (Top-left)
        -0.5f, 0.5f,  -3.5f + thickness / 2,   0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      0.0f, 1.0f,

        // 后面 (Back Face) - 注意Z坐标和法向量
        // 左下 (Bottom-left)
        -0.5f, 0.03f, -3.5f - thickness / 2,   0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,      1.0f, 0.0f, // 纹理坐标可能需要调整
        // 右下 (Bottom-right)
        0.5f, 0.03f, -3.5f - thickness / 2,    0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        // 右上 (Top-right)
        0.5f, 0.5f,  -3.5f - thickness / 2,    0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,      0.0f, 1.0f,
        // 左上 (Top-left)
        -0.5f, 0.5f,  -3.5f - thickness / 2,   0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,      1.0f, 1.0f,

        // 顶面 (Top Face) - 注意Y坐标和法向量
        // 左前上 (Top-left-front)
        -0.5f, 0.5f,  -3.5f + thickness / 2,   0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,      0.0f, 1.0f,
        // 右前上 (Top-right-front)
        0.5f, 0.5f,  -3.5f + thickness / 2,    0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        // 右后上 (Top-right-back)
        0.5f, 0.5f,  -3.5f - thickness / 2,    0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        // 左后上 (Top-left-back)
        -0.5f, 0.5f,  -3.5f - thickness / 2,   0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,      0.0f, 0.0f,

        // 底面 (Bottom Face) - 注意Y坐标和法向量
        // 左前下 (Bottom-left-front)
        -0.5f, 0.03f, -3.5f + thickness / 2,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        // 右前下 (Bottom-right-front)
        0.5f, 0.03f, -3.5f + thickness / 2,    0.0f, -1.0f, 0.0f,     0.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        // 右后下 (Bottom-right-back)
        0.5f, 0.03f, -3.5f - thickness / 2,    0.0f, -1.0f, 0.0f,     0.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        // 左后下 (Bottom-left-back)
        -0.5f, 0.03f, -3.5f - thickness / 2,   0.0f, -1.0f, 0.0f,     0.0f, 0.0f, 0.0f,      0.0f, 1.0f,

        // 左侧面 (Left Face) - 注意X坐标和法向量
        // 左后下 (Bottom-left-back)
        -0.5f, 0.03f, -3.5f - thickness / 2,   -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        // 左前下 (Bottom-left-front)
        -0.5f, 0.03f, -3.5f + thickness / 2,   -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        // 左前上 (Top-left-front)
        -0.5f, 0.5f,  -3.5f + thickness / 2,   -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        // 左后上 (Top-left-back)
        -0.5f, 0.5f,  -3.5f - thickness / 2,   -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,      0.0f, 1.0f,

        // 右侧面 (Right Face) - 注意X坐标和法向量
        // 右前下 (Bottom-right-front)
        0.5f, 0.03f, -3.5f + thickness / 2,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        // 右后下 (Bottom-right-back)
        0.5f, 0.03f, -3.5f - thickness / 2,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        // 右后上 (Top-right-back)
        0.5f, 0.5f,  -3.5f - thickness / 2,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        // 右前上 (Top-right-front)
        0.5f, 0.5f,  -3.5f + thickness / 2,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,      0.0f, 1.0f,
    };

    square_.getIndices() = {
        // 前面
        0, 1, 2,
        0, 2, 3,
        // 后面
        4, 5, 6,
        4, 6, 7,
        // 顶面
        8, 9, 10,
        8, 10, 11,
        // 底面
        12, 13, 14,
        12, 14, 15,
        // 左侧面
        16, 17, 18,
        16, 18, 19,
        // 右侧面
        20, 21, 22,
        20, 22, 23
    };

    square_.getTexturePath() = "../../media//textures/fence.png";
}


void Fence::setup()
{
    square_.setup();
}

void Fence::draw(Shader& shader, const glm::mat4& modelMatrix) const
{
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    // 后方
    for (int i = -2; i<= 2;i++)
    {
        square_.draw(shader, glm::translate(modelMatrix, glm::vec3(static_cast<float>(i), 0.0f, 0.0f)));
    }
	// 前方
    for (int i = -2; i <= 2; i++)
    {
        if (i!= 0)
        {
            square_.draw(shader, glm::translate(modelMatrix, glm::vec3(static_cast<float>(i), 0.0f, 7.0f)));
        }
    }
    float angle_degrees = 90.0f;
    float angle_radians = glm::radians(angle_degrees);
    glm::vec3 rotation_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 model = glm::rotate(modelMatrix, angle_radians, rotation_axis);
    for (int i = -3; i <= 3; i++)
    {
        square_.draw(shader, glm::translate(model, glm::vec3(static_cast<float>(i), 0.0f, 1.0f)));
    }
    for (int i = -3; i <= 3; i++)
    {
        square_.draw(shader, glm::translate(model, glm::vec3(static_cast<float>(i), 0.0f, 6.0f)));
    }
}