#include "base/sphere.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Sphere::Sphere(float radius, int segments, const glm::vec3& color, std::string texture_path):
radius_(radius), segments_(segments), color_(color), texture_path_(texture_path)
{
    square_.getTexturePath() = texture_path;
	generateSphere();
}

void Sphere::generateSphere()
{
    auto& vertices = square_.getVertices();
    auto& indices = square_.getIndices();

    // ����ͨ����γ������
    for (int i = 0; i <= segments_; ++i) { // γ�� (stacks)
        float V = (float)i / (float)segments_;
        float phi = V * M_PI;

        for (int j = 0; j <= segments_; ++j) { // ���� (slices)
            float U = (float)j / (float)segments_;
            float theta = U * (M_PI * 2); // ���ȴ� 0 �� 2*PI

            // λ�� (Position)
            // x = R * sin(phi) * cos(theta)
            // y = R * cos(phi)
            // z = R * sin(phi) * sin(theta)
            float x_pos = radius_ * std::sin(phi) * std::cos(theta);
            float y_pos = radius_ * std::cos(phi);
            float z_pos = radius_ * std::sin(phi) * std::sin(theta);

            // ���� (Normal)
            // ����������ԭ������壬���߾��ǹ�һ�����λ������
            glm::vec3 normal = glm::normalize(glm::vec3(x_pos, y_pos, z_pos));

            // �������� (Texture Coordinates)
            // U: 0-1 ��Ӧ 0-2PI �ľ���
            // V: 0-1 ��Ӧ 0-PI ��γ��
            float u_tex = 1.0f - U;
            float v_tex = 1.0f - V;

            vertices.push_back(x_pos);
            vertices.push_back(y_pos);
            vertices.push_back(z_pos);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(color_.r);
            vertices.push_back(color_.g);
            vertices.push_back(color_.b);

            vertices.push_back(u_tex);
            vertices.push_back(v_tex);
        }
    }
    // ��������
    for (int i = 0; i < segments_; ++i) { // γ�Ȼ�
        for (int j = 0; j < segments_; ++j) { // ���ȶ�
            // p1, p2, p3, p4 �γ�һ���ı��Σ����������������
            // ���ǵ�ǰγ�Ȼ�����һ��γ�Ȼ��Ķ���
            // ÿ��γ�Ȼ��� (segments_ + 1) ������ (��Ϊ��β�غ�)
            unsigned int p1 = i * (segments_ + 1) + j;
            unsigned int p2 = p1 + (segments_ + 1);
            unsigned int p3 = p1 + 1;
            unsigned int p4 = p2 + 1;

            // ���ڷǼ��������γ�����������
            // ��һ��������
            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);

            // �ڶ���������
            indices.push_back(p3);
            indices.push_back(p2);
            indices.push_back(p4);
        }
    }
}

void Sphere::setup()
{
    square_.setup();
}

void Sphere::draw(Shader& shader, const glm::mat4& modelMatrix) const
{
    square_.draw(shader, modelMatrix);
}