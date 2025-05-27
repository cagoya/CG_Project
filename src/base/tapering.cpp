#include "base/tapering.h"
#include <iostream>
#include <glm/glm.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Tapering::Tapering(float radius_, float height_, int segments_, const glm::vec3& color, std::string texture_path) :
    radius_(radius_), height_(height_), segments_(segments_), color_(color), texture_path_(texture_path)
{
    square_.getTexturePath() = texture_path;
    generateTapering();
}

void Tapering::generateTapering()
{
    auto& vertices = square_.getVertices();
    auto& indices = square_.getIndices();
    float bottomZ = -height_ / 2.0f;
    float topZ = height_ / 2.0f;
    float coneTipX = 0.0f;
    float coneTipY = 0.0f;
    float coneTipZ = topZ;

    // --- ���ɵ��� ---
    // ����Բ��
    // λ�ã�(0, 0, bottomZ)
    // ���ߣ�(0, 0, -1)
    unsigned int baseCenterVertexIndex = 0;
    vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(bottomZ);
    vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(-1.0f);
    vertices.push_back(color_.r); vertices.push_back(color_.g); vertices.push_back(color_.b);
    vertices.push_back(0.5f); vertices.push_back(0.5f);

    // ����Բ���ϵĶ���
    for (int i = 0; i < segments_; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / segments_;
        float x = radius_ * cos(angle);
        float y = radius_ * sin(angle);

        // λ�ã�(x, y, bottomZ)
        // ���ߣ�(0, 0, -1)
        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);

        vertices.push_back(x); vertices.push_back(y); vertices.push_back(bottomZ);
        vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(-1.0f);
        vertices.push_back(color_.r); vertices.push_back(color_.g); vertices.push_back(color_.b);
        vertices.push_back(u); vertices.push_back(v);
    }

    // ���ɵ��������
    for (int i = 0; i < segments_; ++i) {
        indices.push_back(baseCenterVertexIndex);
        indices.push_back(baseCenterVertexIndex + 1 + i);
        indices.push_back(baseCenterVertexIndex + 1 + ((i + 1) % segments_));
    }

    // --- ���ɲ��� ---
    // �������Բ׶�Ķ���
    unsigned int coneTipVertexIndex = vertices.size() / 11; // ÿ��������11������
    vertices.push_back(coneTipX); vertices.push_back(coneTipY); vertices.push_back(coneTipZ);
    vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(1.0f);
    vertices.push_back(0.6f); vertices.push_back(0.2f); vertices.push_back(0.2f);
    vertices.push_back(0.5f); vertices.push_back(0.0f);

    // ����Բ���ϵĶ���
    float invLength = 1.0f / std::sqrt(radius_ * radius_ + height_ * height_);
    float normalZComponent = radius_ * invLength;
    float normalXYComponent = height_ * invLength;

    for (int i = 0; i < segments_; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / segments_;
        float x = radius_ * cos(angle);
        float y = radius_ * sin(angle);

        // ���淨��
        float nx = cos(angle) * normalXYComponent;
        float ny = sin(angle) * normalXYComponent;
        float nz = normalZComponent;

        float u = static_cast<float>(i) / segments_;
        float v = 1.0f; 

        vertices.push_back(x); vertices.push_back(y); vertices.push_back(bottomZ);
        vertices.push_back(nx); vertices.push_back(ny); vertices.push_back(nz);
        vertices.push_back(color_.r); vertices.push_back(color_.g); vertices.push_back(color_.b);
        vertices.push_back(u); vertices.push_back(v);
    }

    // ���ɲ��������
    unsigned int sideBaseStartIndex = coneTipVertexIndex + 1;

    for (int i = 0; i < segments_; ++i) {
        indices.push_back(coneTipVertexIndex);
        indices.push_back(sideBaseStartIndex + i);
        indices.push_back(sideBaseStartIndex + ((i + 1) % segments_));
    }

}

void Tapering::setup()
{
    square_.setup();
}

void Tapering::draw(Shader& shader, const glm::mat4& modelMatrix) const
{
    square_.draw(shader, modelMatrix);
}