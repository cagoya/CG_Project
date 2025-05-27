#include "base/Column.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Column::Column(float radius, float height, int segments, const glm::vec3& color, std::string texture_path)
    :radius_(radius), height_(height), segments_(segments), color_(color) {
    square_.getTexturePath() = texture_path;
    // 生成圆柱体侧面数据
    generateColumnSideData();
}

// 生成圆柱体侧面顶点数据
void Column::generateColumnSideData() {
    auto& vertices = square_.getVertices();
    auto& indices = square_.getIndices();
    // 侧面顶点的生成逻辑
    for (int i = 0; i <= segments_; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments_);
        float x_norm = cos(angle);
        float z_norm = sin(angle);

        float u_coord = static_cast<float>(i) / static_cast<float>(segments_);

        // 顶部环的顶点
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

        // 底部环的顶点
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

    // 生成索引数据与之前相同
    for (int i = 0; i < segments_; ++i) {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) * 2);

        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) * 2 + 1);
        indices.push_back((i + 1) * 2);
    }

    // 顶部圆心
	    unsigned int top_center_idx = vertices.size() / 11; // 每个顶点有11个分量
	    vertices.push_back(0.0f);                 // posX
	    vertices.push_back(height_);        // posY
	    vertices.push_back(0.0f);                 // posZ
	    vertices.push_back(0.0f);                 // normX (顶部法线朝上)
	    vertices.push_back(1.0f);                 // normY
	    vertices.push_back(0.0f);                 // normZ
	    vertices.push_back(color_.r);
	    vertices.push_back(color_.g);
	    vertices.push_back(color_.b);
	    vertices.push_back(0.5f);                 // texU (中心)
	    vertices.push_back(0.5f);                 // texV (中心)

	    for (int i = 0; i <= segments_; ++i) {
	        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments_);
	        float x = radius_ * cos(angle);
	        float z = radius_ * sin(angle);

	        // 归一化 x 和 z 到 [0, 1] 范围，用于纹理坐标
	        float u_tex = (x / radius_ + 1.0f) / 2.0f;
	        float v_tex = (z / radius_ + 1.0f) / 2.0f;

	        vertices.push_back(x);                     // posX
	        vertices.push_back(height_);        // posY
	        vertices.push_back(z);                     // posZ
	        vertices.push_back(0.0f);                  // normX (顶部法线朝上)
	        vertices.push_back(1.0f);                  // normY
	        vertices.push_back(0.0f);                  // normZ
	        vertices.push_back(color_.r);
	        vertices.push_back(color_.g);
	        vertices.push_back(color_.b);
	        vertices.push_back(u_tex);                 // texU
	        vertices.push_back(v_tex);                 // texV
	    }

	    // 顶部圆面索引数据
	    for (int i = 0; i < segments_; ++i) {
	        indices.push_back(top_center_idx);
	        indices.push_back(top_center_idx + 1 + i);
	        indices.push_back(top_center_idx + 1 + ((i + 1) % (segments_ + 1))); // 确保最后一个三角形闭合
	    }

    // ----------------------- 底部圆面顶点的生成逻辑 -----------------------
	    // 底部圆心
	    unsigned int bottom_center_idx = vertices.size() / 11;
	    vertices.push_back(0.0f);                 // posX
	    vertices.push_back(0.0f);       // posY
	    vertices.push_back(0.0f);                 // posZ
	    vertices.push_back(0.0f);                 // normX (底部法线朝下)
	    vertices.push_back(-1.0f);                // normY
	    vertices.push_back(0.0f);                 // normZ
	    vertices.push_back(color_.r);
	    vertices.push_back(color_.g);
	    vertices.push_back(color_.b);
	    vertices.push_back(0.5f);                 // texU (中心)
	    vertices.push_back(0.5f);                 // texV (中心)

	    for (int i = 0; i <= segments_; ++i) {
	        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments_);
	        float x = radius_ * cos(angle);
	        float z = radius_ * sin(angle);

	        float u_tex = (x / radius_ + 1.0f) / 2.0f;
	        float v_tex = (z / radius_ + 1.0f) / 2.0f;

	        vertices.push_back(x);                     // posX
	        vertices.push_back(0.0f);       // posY
	        vertices.push_back(z);                     // posZ
	        vertices.push_back(0.0f);                  // normX (底部法线朝下)
	        vertices.push_back(-1.0f);                 // normY
	        vertices.push_back(0.0f);                  // normZ
	        vertices.push_back(color_.r);
	        vertices.push_back(color_.g);
	        vertices.push_back(color_.b);
	        vertices.push_back(u_tex);                 // texU
	        vertices.push_back(v_tex);                 // texV
	    }

	    // 底部圆面索引数据
	    // 注意底部圆面需要逆时针绘制才能保证法线朝外
	    for (int i = 0; i < segments_; ++i) {
	        indices.push_back(bottom_center_idx);
	        indices.push_back(bottom_center_idx + 1 + ((i + 1) % (segments_ + 1))); // 逆时针
	        indices.push_back(bottom_center_idx + 1 + i);
	    }
}

void Column::setup()
{
    square_.setup();
}

void Column::draw(Shader& shader, const glm::mat4& modelMatrix) const
{
    square_.draw(shader, modelMatrix);
}