#include "inside/chair.h"

Chair::Chair() : column_(Column(0.01f, 0.239f, 10, glm::vec3(0.0f, 0.0f, 0.0f), "../../media/textures/muwen.jpg"))
{
    square_.getVertices() = {
        // --- λ�� ---        --- ���� ---        --- ��ɫ ---        --- �������� ---
        // ǰ�� (z=0.25)
        -0.10f,  0.24f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  0.0f, 0.0f, // 0
         0.05f,  0.24f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  1.0f, 0.0f, // 1
         0.05f,  0.25f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  1.0f, 1.0f, // 2
        -0.10f,  0.25f,  0.10f,  0.0f,  0.0f,  1.0f,  0.8f, 0.2f, 0.2f,  0.0f, 1.0f, // 3

        // ���� (z=-0.25)
        -0.10f,  0.24f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  1.0f, 0.0f, // 4
         0.05f,  0.24f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  0.0f, 0.0f, // 5
         0.05f,  0.25f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  0.0f, 1.0f, // 6
        -0.10f,  0.25f, -0.10f,  0.0f,  0.0f, -1.0f,  0.2f, 0.8f, 0.2f,  1.0f, 1.0f, // 7

        // ���� (x=-0.25)
        -0.10f,  0.25f,  0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  1.0f, 0.0f, // 8 (��Ӧǰ-��-��)
        -0.10f,  0.25f, -0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  1.0f, 1.0f, // 9 (��Ӧ��-��-��)
        -0.10f,  0.24f, -0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  0.0f, 1.0f, // 10 (��Ӧ��-��-��)
        -0.10f,  0.24f,  0.10f, -1.0f,  0.0f,  0.0f,  0.2f, 0.2f, 0.8f,  0.0f, 0.0f, // 11 (��Ӧǰ-��-��)

        // ���� (x=0.25)
         0.05f,  0.25f,  0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  1.0f, 0.0f, // 12 (��Ӧǰ-��-��)
         0.05f,  0.25f, -0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  1.0f, 1.0f, // 13 (��Ӧ��-��-��)
         0.05f,  0.24f, -0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  0.0f, 1.0f, // 14 (��Ӧ��-��-��)
         0.05f,  0.24f,  0.10f,  1.0f,  0.0f,  0.0f,  0.8f, 0.8f, 0.2f,  0.0f, 0.0f, // 15 (��Ӧǰ-��-��)

         // ���� (y=0.25)
         -0.10f,  0.25f, -0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  0.0f, 1.0f, // 16 (��Ӧ��-��-��)
          0.05f,  0.25f, -0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  1.0f, 1.0f, // 17 (��Ӧ��-��-��)
          0.05f,  0.25f,  0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  1.0f, 0.0f, // 18 (��Ӧǰ-��-��)
         -0.10f,  0.25f,  0.10f,  0.0f,  1.0f,  0.0f,  0.2f, 0.8f, 0.8f,  0.0f, 0.0f, // 19 (��Ӧǰ-��-��)

         // ���� (y=-0.25)
         -0.10f,  0.24f, -0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  0.0f, 1.0f, // 20 (��Ӧ��-��-��)
          0.05f,  0.24f, -0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  1.0f, 1.0f, // 21 (��Ӧ��-��-��)
          0.05f,  0.24f,  0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  1.0f, 0.0f, // 22 (��Ӧǰ-��-��)
         -0.10f,  0.24f,  0.10f,  0.0f, -1.0f,  0.0f,  0.8f, 0.2f, 0.8f,  0.0f, 0.0f  // 23 (��Ӧǰ-��-��)
    };

    // �������ݣ����������ÿ�������εĶ���˳��
    square_.getIndices() = {
        // ǰ��
        0, 1, 2,
        2, 3, 0,

        // ����
        4, 5, 6,
        6, 7, 4,

        // ����
        8, 9, 10,
        10, 11, 8,

        // ����
        12, 13, 14,
        14, 15, 12,

        // ����
        16, 17, 18,
        18, 19, 16,

        // ����
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