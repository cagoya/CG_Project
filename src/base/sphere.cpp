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

    // 球体通过经纬线生成
    for (int i = 0; i <= segments_; ++i) { // 纬度 (stacks)
        float V = (float)i / (float)segments_;
        float phi = V * M_PI;

        for (int j = 0; j <= segments_; ++j) { // 经度 (slices)
            float U = (float)j / (float)segments_;
            float theta = U * (M_PI * 2); // 经度从 0 到 2*PI

            // 位置 (Position)
            // x = R * sin(phi) * cos(theta)
            // y = R * cos(phi)
            // z = R * sin(phi) * sin(theta)
            float x_pos = radius_ * std::sin(phi) * std::cos(theta);
            float y_pos = radius_ * std::cos(phi);
            float z_pos = radius_ * std::sin(phi) * std::sin(theta);

            // 法线 (Normal)
            // 对于球心在原点的球体，法线就是归一化后的位置向量
            glm::vec3 normal = glm::normalize(glm::vec3(x_pos, y_pos, z_pos));

            // 纹理坐标 (Texture Coordinates)
            // U: 0-1 对应 0-2PI 的经度
            // V: 0-1 对应 0-PI 的纬度
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
    // 生成索引
    for (int i = 0; i < segments_; ++i) { // 纬度环
        for (int j = 0; j < segments_; ++j) { // 经度段
            // p1, p2, p3, p4 形成一个四边形，由两个三角形组成
            // 考虑当前纬度环和下一个纬度环的顶点
            // 每个纬度环有 (segments_ + 1) 个顶点 (因为首尾重合)
            unsigned int p1 = i * (segments_ + 1) + j;
            unsigned int p2 = p1 + (segments_ + 1);
            unsigned int p3 = p1 + 1;
            unsigned int p4 = p2 + 1;

            // 对于非极点区域，形成两个三角形
            // 第一个三角形
            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);

            // 第二个三角形
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