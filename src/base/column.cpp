#include "base/Column.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Column::Column(float radius, float height, int segments, const glm::vec3& color, std::string texture_path)
    :radius_(radius), height_(height), segments_(segments), color_(color) {
    square_.getTexturePath() = texture_path;
    generateColumnSideData();
}

void Column::generateColumnSideData() {
    auto& vertices = square_.getVertices();
    auto& indices = square_.getIndices();
    for (int i = 0; i <= segments_; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments_);
        float x_norm = cos(angle);
        float z_norm = sin(angle);

        float u_coord = static_cast<float>(i) / static_cast<float>(segments_);

        vertices.push_back(radius_ * x_norm);
        vertices.push_back(height_);
        vertices.push_back(radius_ * z_norm);
        vertices.push_back(x_norm);
        vertices.push_back(0.0f);
        vertices.push_back(z_norm);
        vertices.push_back(color_.r);
        vertices.push_back(color_.g);
        vertices.push_back(color_.b);
        vertices.push_back(u_coord);
        vertices.push_back(1.0f);

        vertices.push_back(radius_ * x_norm);
        vertices.push_back(0.0f);
        vertices.push_back(radius_ * z_norm);
        vertices.push_back(x_norm);
        vertices.push_back(0.0f);
        vertices.push_back(z_norm);
        vertices.push_back(color_.r);
        vertices.push_back(color_.g);
        vertices.push_back(color_.b);
        vertices.push_back(u_coord);
        vertices.push_back(0.0f);
    }

    for (int i = 0; i < segments_; ++i) {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) * 2);

        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) * 2 + 1);
        indices.push_back((i + 1) * 2);
    }

    // ����Բ��
	    unsigned int top_center_idx = vertices.size() / 11; // ÿ��������11������
	    vertices.push_back(0.0f);                 // posX
	    vertices.push_back(height_);        // posY
	    vertices.push_back(0.0f);                 // posZ
	    vertices.push_back(0.0f);                 // normX (�������߳���)
	    vertices.push_back(1.0f);                 // normY
	    vertices.push_back(0.0f);                 // normZ
	    vertices.push_back(color_.r);
	    vertices.push_back(color_.g);
	    vertices.push_back(color_.b);
	    vertices.push_back(0.5f);                 // texU (����)
	    vertices.push_back(0.5f);                 // texV (����)

	    for (int i = 0; i <= segments_; ++i) {
	        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments_);
	        float x = radius_ * cos(angle);
	        float z = radius_ * sin(angle);

	        // ��һ�� x �� z �� [0, 1] ��Χ��������������
	        float u_tex = (x / radius_ + 1.0f) / 2.0f;
	        float v_tex = (z / radius_ + 1.0f) / 2.0f;

	        vertices.push_back(x);                     // posX
	        vertices.push_back(height_);        // posY
	        vertices.push_back(z);                     // posZ
	        vertices.push_back(0.0f);                  // normX (�������߳���)
	        vertices.push_back(1.0f);                  // normY
	        vertices.push_back(0.0f);                  // normZ
	        vertices.push_back(color_.r);
	        vertices.push_back(color_.g);
	        vertices.push_back(color_.b);
	        vertices.push_back(u_tex);                 // texU
	        vertices.push_back(v_tex);                 // texV
	    }

	    // ����Բ����������
	    for (int i = 0; i < segments_; ++i) {
	        indices.push_back(top_center_idx);
	        indices.push_back(top_center_idx + 1 + i);
	        indices.push_back(top_center_idx + 1 + ((i + 1) % (segments_ + 1))); // ȷ�����һ�������αպ�
	    }

    // ----------------------- �ײ�Բ�涥��������߼� -----------------------
	    // �ײ�Բ��
	    unsigned int bottom_center_idx = vertices.size() / 11;
	    vertices.push_back(0.0f);                 // posX
	    vertices.push_back(0.0f);       // posY
	    vertices.push_back(0.0f);                 // posZ
	    vertices.push_back(0.0f);                 // normX (�ײ����߳���)
	    vertices.push_back(-1.0f);                // normY
	    vertices.push_back(0.0f);                 // normZ
	    vertices.push_back(color_.r);
	    vertices.push_back(color_.g);
	    vertices.push_back(color_.b);
	    vertices.push_back(0.5f);                 // texU (����)
	    vertices.push_back(0.5f);                 // texV (����)

	    for (int i = 0; i <= segments_; ++i) {
	        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments_);
	        float x = radius_ * cos(angle);
	        float z = radius_ * sin(angle);

	        float u_tex = (x / radius_ + 1.0f) / 2.0f;
	        float v_tex = (z / radius_ + 1.0f) / 2.0f;

	        vertices.push_back(x);                     // posX
	        vertices.push_back(0.0f);       // posY
	        vertices.push_back(z);                     // posZ
	        vertices.push_back(0.0f);                  // normX (�ײ����߳���)
	        vertices.push_back(-1.0f);                 // normY
	        vertices.push_back(0.0f);                  // normZ
	        vertices.push_back(color_.r);
	        vertices.push_back(color_.g);
	        vertices.push_back(color_.b);
	        vertices.push_back(u_tex);                 // texU
	        vertices.push_back(v_tex);                 // texV
	    }

	    // �ײ�Բ����������
	    // ע��ײ�Բ����Ҫ��ʱ����Ʋ��ܱ�֤���߳���
	    for (int i = 0; i < segments_; ++i) {
	        indices.push_back(bottom_center_idx);
	        indices.push_back(bottom_center_idx + 1 + ((i + 1) % (segments_ + 1))); // ��ʱ��
	        indices.push_back(bottom_center_idx + 1 + i);
	    }
}

void Column::setup()
{
    square_.setup();
}

void Column::draw(Shader& shader, const glm::mat4& modelMatrix,bool useTexture) const
{
    square_.draw(shader, modelMatrix, useTexture);
}