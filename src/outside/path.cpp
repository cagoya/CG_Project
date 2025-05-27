#include"outside/path.h"

Path::Path()
{
    square_.getVertices() = {
        // λ�� (Position)             // ������ (Normal)     // ��ɫ (Color)         // �������� (Texture)
        // ���� (Bottom-left)
        -0.15f, 0.01f, 0.502f,           0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // ���� (Bottom-right)
        0.15f, 0.01f, 0.502f,           0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // ���� (Top-right)
        0.15f, 0.01f,  1.75f,           0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // ���� (Top-left)
        -0.15f, 0.01f,  1.75f,           0.0f, 1.0f, 0.0f,      1.0f, 1.0f, 1.0f,      0.0f, 1.0f
    };

    square_.getIndices() = {
        0, 1, 2,
        0, 2, 3
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