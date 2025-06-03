#include "outside/door.h"
#include <glad/gl.h>
#include<stb_image.h>
#include<iostream>
#include <glm/gtc/type_ptr.hpp>

Door::Door() : DrawObject(),square_()
{
	// 定义门的厚度参数
	float door_thickness = 0.01f; // 你可以根据需要修改这个值，例如 0.1f, 0.02f 等

	// 定义门的前表面Z坐标
	float z_front = 0.501f;

	// 计算门的后表面Z坐标
	float z_back = z_front - door_thickness;

	square_.getVertices() = {
		// 顶点格式: x, y, z,   nx, ny, nz,   r, g, b,   u, v

		// 正面 (Front Face) - Z 坐标为 z_front
		// 0: 左下前
		-0.1f, 0.0f, z_front,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		// 1: 右下前
		 0.1f, 0.0f, z_front,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		// 2: 右上前
		 0.1f, 0.3f, z_front,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		// 3: 左上前
		-0.1f, 0.3f, z_front,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

		// 背面 (Back Face) - Z 坐标为 z_back
		// 4: 左下后
		-0.1f, 0.0f, z_back,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		// 5: 右下后
		 0.1f, 0.0f, z_back,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		// 6: 右上后
		 0.1f, 0.3f, z_back,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		// 7: 左上后
		-0.1f, 0.3f, z_back,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
	};

	square_.getIndices() = {
		// 正面 (Front face): P0, P1, P2, P3
		0, 1, 2,
		0, 2, 3,

		// 背面 (Back face): P4, P5, P6, P7
		4, 6, 5,
		4, 7, 6,

		// 顶部 (Top face): P3, P2, P6, P7
		3, 2, 6,
		3, 6, 7,

		// 底部 (Bottom face): P0, P4, P5, P1
		0, 4, 5,
		0, 5, 1,

		// 左侧 (Left face): P0, P3, P7, P4
		0, 3, 7,
		0, 7, 4,

		// 右侧 (Right face): P1, P5, P6, P2
		1, 5, 6,
		1, 6, 2
	};

	square_.getIndices() = {
		// 正面 (Front face): P0, P1, P2, P3 (原门面)
		0, 1, 2,  // P0, P1, P2
		0, 2, 3,  // P0, P2, P3

		// 背面 (Back face): P4, P5, P6, P7 (与正面相对)
		// 注意索引顺序，以确保法线朝外
		4, 6, 5,  // P4, P6, P5
		4, 7, 6,  // P4, P7, P6

		// 顶部 (Top face): P3, P2, P6, P7
		3, 2, 6,  // P3, P2, P6
		3, 6, 7,  // P3, P6, P7

		// 底部 (Bottom face): P0, P4, P5, P1
		0, 4, 5,  // P0, P4, P5
		0, 5, 1,  // P0, P5, P1

		// 左侧 (Left face): P0, P3, P7, P4
		0, 3, 7,  // P0, P3, P7
		0, 7, 4,  // P0, P7, P4

		// 右侧 (Right face): P1, P5, P6, P2
		1, 5, 6,  // P1, P5, P6
		1, 6, 2   // P1, P6, P2
	};

    square_.getTexturePath() = "../../media/textures/door.png";
}

void Door::setup() {
    square_.setup();
}

void Door::draw(Shader& shader, const glm::mat4& modelMatrix,bool useTexture) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix, useTexture);
}
