#include "inside/calligraphy.h"
#include<iostream>

Calligraphy::Calligraphy()
{
    square_.getVertices() = {
        // λ�� (Position)             // ������ (Normal)     // ��ɫ (Color)         // �������� (Texture)
        // ���� (Bottom-left)
        -0.25f, 0.5f, -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.5f,
        // ���� (Bottom-right)
        0.25f, 0.5f, -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.5f, 1.0f,
        // ���� (Top-right)
        0.25f, 0.7f,  -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.5f, 0.0f,
        // ���� (Top-left)
        -0.25f, 0.7f,  -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f
    };

    square_.getIndices() = {
        0, 1, 2,
        0, 2, 3
    };

    square_.getTexturePath() = "../../media/textures/paper.jpg";
}

void Calligraphy::setup() {
    square_.setup();
}

void Calligraphy::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
}
