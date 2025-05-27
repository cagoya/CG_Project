#include "outside/fence.h"

Fence::Fence()
{
    square_.getVertices() = {
        // λ�� (Position)             // ������ (Normal)     // ��ɫ (Color)         // �������� (Texture)
        // ���� (Bottom-left)
        -0.5f, 0.03f, -3.5f,           0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        // ���� (Bottom-right)
        0.5f, 0.03f, -3.5f,           0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        // ���� (Top-right)
        0.5f, 0.5f,  -3.5f,           0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        // ���� (Top-left)
        -0.5f, 0.5f,  -3.5f,           0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 0.0f,      0.0f, 1.0f,
    };

    square_.getIndices() = {
        0, 1, 2,
        0, 2, 3
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
    // ��
    for (int i = -2; i<= 2;i++)
    {
        square_.draw(shader, glm::translate(modelMatrix, glm::vec3(static_cast<float>(i), 0.0f, 0.0f)));
    }
	// ǰ��
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