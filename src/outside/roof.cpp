#include "outside/roof.h"
#include <vector>

Roof::Roof()
{
    float thickness = 0.02f;

    // 定义屋顶顶部的高度基础值和脊线高度
    float topBaseY = 0.475f;
    float topRidgeY = 0.625f;

    // 根据 thickness 计算底部的高度
    float bottomBaseY = topBaseY + thickness;
    float bottomRidgeY = topRidgeY + thickness;

    square_.getVertices() = {
        // 左斜面 (Left Oblique Face) - 顶部
        // 0: 左底前 (top-front-left)
        -0.6f, topBaseY,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        // 1: 左底后 (top-back-left)
        -0.6f, topBaseY, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        // 2: 顶脊后 (top-back-ridge)
        0.0f, topRidgeY, -0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        // 3: 顶脊前 (top-front-ridge)
        0.0f, topRidgeY,  0.6f,   -0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

        // 右斜面 (Right Oblique Face) - 顶部
        // 4: 右底前 (top-front-right)
        0.6f, topBaseY,  0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        // 5: 右底后 (top-back-right)
        0.6f, topBaseY, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        // 6: 顶脊后 (同 2) (top-back-ridge)
        0.0f, topRidgeY, -0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        // 7: 顶脊前 (同 3) (top-front-ridge)
        0.0f, topRidgeY,  0.6f,    0.242f, 0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

        // --- 增加底部顶点以创建厚度 ---
        // 左斜面 (Left Oblique Face) - 底部
        // 8: 左底前 (bottom-front-left)
        -0.6f, bottomBaseY,  0.6f,   -0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        // 9: 左底后 (bottom-back-left)
        -0.6f, bottomBaseY, -0.6f,   -0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        // 10: 顶脊后 (bottom-back-ridge)
        0.0f, bottomRidgeY, -0.6f,   -0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        // 11: 顶脊前 (bottom-front-ridge)
        0.0f, bottomRidgeY,  0.6f,   -0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

        // 右斜面 (Right Oblique Face) - 底部
        // 12: 右底前 (bottom-front-right)
        0.6f, bottomBaseY,  0.6f,    0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        // 13: 右底后 (bottom-back-right)
        0.6f, bottomBaseY, -0.6f,    0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        // 14: 顶脊后 (同 10) (bottom-back-ridge)
        0.0f, bottomRidgeY, -0.6f,    0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        // 15: 顶脊前 (同 11) (bottom-front-ridge)
        0.0f, bottomRidgeY,  0.6f,    0.242f, -0.970f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };

    square_.getIndices() = {
        // 顶部两个斜面 (Top two oblique faces)
        0, 1, 2,  // 左斜面 1
        0, 2, 3,  // 左斜面 2
        4, 5, 6,  // 右斜面 1
        4, 6, 7,  // 右斜面 2

        // 底部两个斜面 (Bottom two oblique faces) - 注意法线方向，可能需要调整
        8, 10, 9,  // 左斜面 1 (逆时针以保持正面)
        8, 11, 10, // 左斜面 2
        12, 14, 13, // 右斜面 1
        12, 15, 14, // 右斜面 2

        // 前面 (Front Face)
        // 左前边缘
        0, 3, 11,
        0, 11, 8,
        // 右前边缘
        4, 12, 15,
        4, 15, 7,
        // 中心前边缘 (连接左右斜面的脊线部分)
        3, 7, 15,
        3, 15, 11,

        // 后面 (Back Face)
        // 左后边缘
        1, 9, 10,
        1, 10, 2,
        // 右后边缘
        5, 6, 14,
        5, 14, 13,
        // 中心后边缘
        2, 10, 14,
        2, 14, 6,

        // 两侧的侧面 (Side Faces)
        // 左侧面 (Left Side Face) - 连接左斜面顶部和底部边缘
        1, 0, 8,
        1, 8, 9,
        // 右侧面 (Right Side Face) - 连接右斜面顶部和底部边缘
        4, 5, 13,
        4, 13, 12
    };

    square_.getTexturePath() = "../../media/textures/roof.jpg";
}

void Roof::setup() {
    square_.setup();
}

void Roof::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
}