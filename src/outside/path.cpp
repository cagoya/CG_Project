#include"outside/path.h"

Path::Path()
{
    float thickness = 0.02f; // 你可以根据需要调整这个厚度
    float originalY = 0.02f;
    float bottomY = originalY - thickness; // 新的底部Y坐标
    square_.getVertices() = {
        // 位置                 // 法向量             // 颜色         // 纹理坐标

        // 顶面 (Top Face) - 和你原始的 square_ 相同，法向量向上 (0, 1, 0)
        // 左下 (Bottom-left of top face) - 对应原始数据的 0
        -0.15f, originalY, 0.502f,           0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // 右下 (Bottom-right of top face) - 对应原始数据的 1
        0.15f, originalY, 0.502f,           0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // 右上 (Top-right of top face) - 对应原始数据的 2
        0.15f, originalY, 1.75f,            0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // 左上 (Top-left of top face) - 对应原始数据的 3
        -0.15f, originalY, 1.75f,            0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 1.0f,

        // 底面 (Bottom Face) - 法向量向下 (0, -1, 0)
        // 左下 (Bottom-left of bottom face) - 对应原始 0 的底部
        -0.15f, bottomY, 0.502f,           0.0f, -1.0f, 0.0f,     1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // 右下 (Bottom-right of bottom face) - 对应原始 1 的底部
        0.15f, bottomY, 0.502f,           0.0f, -1.0f, 0.0f,     1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // 右上 (Top-right of bottom face) - 对应原始 2 的底部
        0.15f, bottomY, 1.75f,            0.0f, -1.0f, 0.0f,     1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // 左上 (Top-left of bottom face) - 对应原始 3 的底部
        -0.15f, bottomY, 1.75f,            0.0f, -1.0f, 0.0f,     1.0f, 1.0f, 1.0f,      0.0f, 1.0f,

        // 前面 (Front Face, Z = 1.75f) - 法向量向正Z (0, 0, 1)
        // 左下
        -0.15f, bottomY, 1.75f,            0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // 右下
        0.15f, bottomY, 1.75f,            0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // 右上
        0.15f, originalY, 1.75f,            0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // 左上
        -0.15f, originalY, 1.75f,            0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 1.0f,

        // 后面 (Back Face, Z = 0.502f) - 法向量向负Z (0, 0, -1)
        // 左下
        -0.15f, bottomY, 0.502f,           0.0f, 0.0f, -1.0f,     1.0f, 1.0f, 1.0f,      1.0f, 0.0f, // 纹理翻转
        // 右下
        0.15f, bottomY, 0.502f,           0.0f, 0.0f, -1.0f,     1.0f, 1.0f, 1.0f,      0.0f, 0.0f, // 纹理翻转
        // 右上
        0.15f, originalY, 0.502f,           0.0f, 0.0f, -1.0f,     1.0f, 1.0f, 1.0f,      0.0f, 1.0f, // 纹理翻转
        // 左上
        -0.15f, originalY, 0.502f,           0.0f, 0.0f, -1.0f,     1.0f, 1.0f, 1.0f,      1.0f, 1.0f, // 纹理翻转

        // 左面 (Left Face, X = -0.15f) - 法向量向负X (-1, 0, 0)
        // 左下 (后)
        -0.15f, bottomY, 0.502f,           -1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,      1.0f, 0.0f, // 纹理翻转
        // 右下 (前)
        -0.15f, bottomY, 1.75f,            -1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,      0.0f, 0.0f, // 纹理翻转
        // 右上 (前)
        -0.15f, originalY, 1.75f,            -1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,      0.0f, 1.0f, // 纹理翻转
        // 左上 (后)
        -0.15f, originalY, 0.502f,           -1.0f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,      1.0f, 1.0f, // 纹理翻转

        // 右面 (Right Face, X = 0.15f) - 法向量向正X (1, 0, 0)
        // 左下 (后)
        0.15f, bottomY, 0.502f,           1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // 右下 (前)
        0.15f, bottomY, 1.75f,            1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // 右上 (前)
        0.15f, originalY, 1.75f,            1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // 左上 (后)
        0.15f, originalY, 0.502f,           1.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 1.0f
    };

    square_.getIndices() = {
        // 顶面 (Top Face) - 顶点索引 0-3
        0, 1, 2,
        0, 2, 3,

        // 底面 (Bottom Face) - 顶点索引 4-7
        // 注意：为了让法向量朝外，底面的三角形卷绕顺序可能与顶面相反
        4, 6, 5,
        4, 7, 6,

        // 前面 (Front Face) - 顶点索引 8-11
        8, 9, 10,
        8, 10, 11,

        // 后面 (Back Face) - 顶点索引 12-15
        // 注意：法向量向负Z，为了从外面看到，可能需要调整索引顺序
        12, 14, 13,
        12, 15, 14,

        // 左面 (Left Face) - 顶点索引 16-19
        // 注意：法向量向负X，为了从外面看到，可能需要调整索引顺序
        16, 18, 17,
        16, 19, 18,

        // 右面 (Right Face) - 顶点索引 20-23
        20, 21, 22,
        20, 22, 23
    };

    square_.getTexturePath() = "../../media/textures/path.jpg";
}

void Path::setup() {
    square_.setup();
}

void Path::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
}