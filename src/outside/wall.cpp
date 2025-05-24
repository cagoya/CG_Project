#include "outside/wall.h" 
#include <glad/gl.h>
#include <iostream>

Wall::Wall() : DrawObject()
{
    square_.getVertices() = {
		// --- ԭ�ж��� 0-29 ---
		// ���� (Back Face) - Z = -0.5f, Normal: (0.0f, 0.0f, -1.0f)
		// ����
		-0.5f, 0.0f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.5f,
		// ����
		0.5f, 0.0f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.5f, 0.5f,
		// ����
		0.5f, 0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.5f, 1.0f,
		// ����
		-0.5f, 0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

		// ����������� (Front Base Face) - Z = 0.5f, Normal: (0.0f, 0.0f, 1.0f)
		// V4 ����
		-0.5f, 0.0f, 0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		// V5 ����
		0.5f, 0.0f, 0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		// V6 ����
		0.5f, 0.5f, 0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   1.0f, 0.5f,
		// V7 ����
		-0.5f, 0.5f, 0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 0.5f,

		// ���� (Left Face) - X = -0.5f, Normal: (-1.0f, 0.0f, 0.0f)
		-0.5f, 0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.5f,
		-0.5f, 0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.5f, 0.5f,
		-0.5f, 0.0f, -0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.5f, 1.0f,
		-0.5f, 0.0f, 0.5f,     -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

		// ���� (Right Face) - X = 0.5f, Normal: (1.0f, 0.0f, 0.0f)
		0.5f, 0.5f, 0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.5f,
		0.5f, 0.0f, -0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		0.5f, 0.0f, 0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.5f, 1.0f,

		// �ݶ�ǰ�������� (Front Roof Triangle) - Normal calculated for a common plane
		-0.5f, 0.5f, 0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 0.5f,
		0.5f, 0.5f, 0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   1.0f, 0.5f,
		0.0f, 0.625f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,   0.5f, 0.625f,

		// �ݶ����������� (Back Roof Triangle) - Normal: (0.0f, 0.0f, -1.0f)
		-0.5f, 0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.5f,
		0.5f, 0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.5f,
		0.0f, 0.625f, -0.5f,  0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.5f, 0.625f,

		// ���� (Top Face) - This refers to the horizontal part, Y = 0.5f, Normal: (0.0f, 1.0f, 0.0f)
		-0.5f, 0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

		// ����С���� (Right Small Rectangle) - X = 0.5f, Normal: (1.0f, 0.0f, 0.0f)
		// This looks like a window or feature on the right side.
		0.5f, 0.175f, -0.15f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		0.5f, 0.175f, 0.15f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.3f, 0.0f,
		0.5f, 0.325f, 0.15f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.3f, 0.3f,
		0.5f, 0.325f, -0.15f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.3f,

		// --- �������� (New Vertices 30-35) ---
		// ��Щ�㶼�� Z = 0.5f ��ƽ���ϣ��������棬���Է������� (0.0f, 0.0f, 1.0f)
		/*30 Hole_LB*/ -0.1f, 0.0f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.4f, 0.0f,
		/*31 Hole_RB*/  0.1f, 0.0f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.6f, 0.0f,
		/*32 Hole_RT*/  0.1f, 0.3f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.6f, 0.3f,
		/*33 Hole_LT*/ -0.1f, 0.3f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.4f, 0.3f,
		/*34 Aux_L*/   -0.5f, 0.3f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.3f,
		/*35 Aux_R*/    0.5f, 0.3f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.3f
    };

// �������ֲ���
square_.getIndices() = {
    // ����
    0, 1, 2,  0, 2, 3,
    // ����
    8, 9,10,  8,10,11,
    // ��������
    15, 27, 28, 15, 28, 12,
    12, 28, 29, 12, 29, 13,
    13, 29, 26, 13, 26, 14,
    14, 26, 27, 14, 27, 15,
    // �ݶ�ǰ��
    16, 17, 18,
    // �ݶ�����
    19, 20, 21,
    // ����
    22, 23, 24,  22, 24, 25,

    // ����ǽ���Ŷ�
    4, 30, 33,
    4, 33, 34,
    31, 5, 35,
    31, 35, 32,
    34, 35, 6,
    34, 6, 7
};

    square_.getTexturePath() = "../../media/textures/wall.jpg";
} 

void Wall::setup() {
    square_.setup();
}


void Wall::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
}