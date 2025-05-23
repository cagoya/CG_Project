#include "inside/table.h" // �������ͷ�ļ�·����ȷ
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

// �������ɫ
const float TC_R = 0.4f, TC_G = 0.2f, TC_B = 0.0f;
// ���ȵ���ɫ
const float LC_R = 0.3f, LC_G = 0.15f, LC_B = 0.0f;

// ��������: X, Y, Z, R, G, B
static const float tableVertices[] = {
    // ���� (8������) - Y��Χ [0.75, 0.80], X��Χ [-0.5, 0.5], Z��Χ [-0.3, 0.3]
    // ���� (Y = 0.75f)
    -0.5f, 0.75f, -0.3f,  TC_R, TC_G, TC_B, // 0 ������
     0.5f, 0.75f, -0.3f,  TC_R, TC_G, TC_B, // 1 ������
     0.5f, 0.75f,  0.3f,  TC_R, TC_G, TC_B, // 2 ǰ����
    -0.5f, 0.75f,  0.3f,  TC_R, TC_G, TC_B, // 3 ǰ����
    // ���� (Y = 0.80f)
    -0.5f, 0.80f, -0.3f,  TC_R, TC_G, TC_B, // 4 ������
     0.5f, 0.80f, -0.3f,  TC_R, TC_G, TC_B, // 5 ������
     0.5f, 0.80f,  0.3f,  TC_R, TC_G, TC_B, // 6 ǰ����
    -0.5f, 0.80f,  0.3f,  TC_R, TC_G, TC_B, // 7 ǰ����

    // --- ������ ---
    // ÿ����8�����㣬Y��Χ [0.0, 0.75]

    // ����1: ��ǰ (LF)
    // X��Χ: [-0.48, -0.43], Y��Χ: [0.0, 0.75], Z��Χ: [0.23, 0.28] (ԭZ=0.28Ϊǰ�����0.05�������)
    // ��ƽ�� (Z = 0.23f)
    -0.48f, 0.00f, 0.23f,  LC_R, LC_G, LC_B, // 8  (LF_BLL - Back Lower Left)
    -0.43f, 0.00f, 0.23f,  LC_R, LC_G, LC_B, // 9  (LF_BLR - Back Lower Right)
    -0.43f, 0.75f, 0.23f,  LC_R, LC_G, LC_B, // 10 (LF_BUR - Back Upper Right)
    -0.48f, 0.75f, 0.23f,  LC_R, LC_G, LC_B, // 11 (LF_BUL - Back Upper Left)
    // ǰƽ�� (Z = 0.28f)
    -0.48f, 0.00f, 0.28f,  LC_R, LC_G, LC_B, // 12 (LF_FLL - Front Lower Left)
    -0.43f, 0.00f, 0.28f,  LC_R, LC_G, LC_B, // 13 (LF_FLR - Front Lower Right)
    -0.43f, 0.75f, 0.28f,  LC_R, LC_G, LC_B, // 14 (LF_FUR - Front Upper Right)
    -0.48f, 0.75f, 0.28f,  LC_R, LC_G, LC_B, // 15 (LF_FUL - Front Upper Left)

    // ����2: ��ǰ (RF)
    // X��Χ: [0.43, 0.48], Y��Χ: [0.0, 0.75], Z��Χ: [0.23, 0.28]
    // ��ƽ�� (Z = 0.23f)
     0.43f, 0.00f, 0.23f,  LC_R, LC_G, LC_B, // 16 (RF_BLL)
     0.48f, 0.00f, 0.23f,  LC_R, LC_G, LC_B, // 17 (RF_BLR)
     0.48f, 0.75f, 0.23f,  LC_R, LC_G, LC_B, // 18 (RF_BUR)
     0.43f, 0.75f, 0.23f,  LC_R, LC_G, LC_B, // 19 (RF_BUL)
     // ǰƽ�� (Z = 0.28f)
      0.43f, 0.00f, 0.28f,  LC_R, LC_G, LC_B, // 20 (RF_FLL)
      0.48f, 0.00f, 0.28f,  LC_R, LC_G, LC_B, // 21 (RF_FLR)
      0.48f, 0.75f, 0.28f,  LC_R, LC_G, LC_B, // 22 (RF_FUR)
      0.43f, 0.75f, 0.28f,  LC_R, LC_G, LC_B, // 23 (RF_FUL)

      // ����3: ��� (LB)
      // X��Χ: [-0.48, -0.43], Y��Χ: [0.0, 0.75], Z��Χ: [-0.28, -0.23] (ԭZ=-0.28Ϊ�󣬺��0.05��ǰ����)
      // ��ƽ�� (Z = -0.28f)
      // ��ƽ�� (Z = -0.28f)
		-0.48f, 0.00f, -0.28f, LC_R, LC_G, LC_B, // 24 (LB_BLL)
		-0.43f, 0.00f, -0.28f, LC_R, LC_G, LC_B, // 25 (LB_BLR)
		-0.43f, 0.75f, -0.28f, LC_R, LC_G, LC_B, // 26 (LB_BUR)
		-0.48f, 0.75f, -0.28f, LC_R, LC_G, LC_B, // 27 (LB_BUL)
		// ǰƽ�� (Z = -0.23f)
		-0.48f, 0.00f, -0.23f, LC_R, LC_G, LC_B, // 28 (LB_FLL)
		-0.43f, 0.00f, -0.23f, LC_R, LC_G, LC_B, // 29 (LB_FLR)
		-0.43f, 0.75f, -0.23f, LC_R, LC_G, LC_B, // 30 (LB_FUR)
		-0.48f, 0.75f, -0.23f, LC_R, LC_G, LC_B, // 31 (LB_FUL)

      // ����4: �Һ� (RB)
      // X��Χ: [0.43, 0.48], Y��Χ: [0.0, 0.75], Z��Χ: [-0.28, -0.23]
      // ��ƽ�� (Z = -0.28f)
       0.43f, 0.00f, -0.28f, LC_R, LC_G, LC_B, // 32 (RB_BLL)
       0.48f, 0.00f, -0.28f, LC_R, LC_G, LC_B, // 33 (RB_BLR)
       0.48f, 0.75f, -0.28f, LC_R, LC_G, LC_B, // 34 (RB_BUR)
       0.43f, 0.75f, -0.28f, LC_R, LC_G, LC_B, // 35 (RB_BUL)
       // ǰƽ�� (Z = -0.23f)
        0.43f, 0.00f, -0.23f, LC_R, LC_G, LC_B, // 36 (RB_FLL)
        0.48f, 0.00f, -0.23f, LC_R, LC_G, LC_B, // 37 (RB_FLR)
        0.48f, 0.75f, -0.23f, LC_R, LC_G, LC_B, // 38 (RB_FUR)
        0.43f, 0.75f, -0.23f, LC_R, LC_G, LC_B  // 39 (RB_FUL)
}; // Total 40 vertices: 8 for tabletop + 4 * 8 for legs. 40 * 6 = 240 floats.

static unsigned int tableIndices[] = {
    // ���� (6�� * 2���� * 3���� = 36 indices)
    // ����
    0, 2, 1, 0, 3, 2,  // (ʹ����ԭ����˳��, 0,1,2, 0,2,3, ��ͨ��CCW�� 0,1,2, 0,2,3���������ΪCCW: 0,2,1, 0,3,2)
    // ���˳��: 0,1,2, 0,2,3 -> 0,1,2,3 �ı���, CCW: 0,1,2 �� 0,2,3 (���1���ң�3������)
    // ����ԭʼ��������ȷ�ģ����Ǳ�������
0, 1, 2, 0, 2, 3,  // �������
4, 5, 6, 4, 6, 7,  // ���涥�� (���ֺ�ԭ��һ��, 4,5,6, 4,6,7 for top face)
// ������ (4���� * 2���� * 3���� = 24 indices)
0, 4, 5, 0, 5, 1,  // ��� (Back face of tabletop)
1, 5, 6, 1, 6, 2,  // �Ҳ� (Right face of tabletop)
2, 6, 7, 2, 7, 3,  // ǰ�� (Front face of tabletop)
3, 7, 4, 3, 4, 0,  // ��� (Left face of tabletop)

// ����1: ��ǰ (LF) - �������� 8-15 (ƫ����Ϊ8)
// ÿ������������Ҫ 6�� * 2���� * 3���� = 36������
// �������: 0=BLL, 1=BLR, 2=BUR, 3=BUL, 4=FLL, 5=FLR, 6=FUR, 7=FUL
// ��(Z=0.23): 8,9,10,   8,10,11
// ǰ(Z=0.28): 12,13,14, 12,14,15 (����ĳɱ�׼������������˳���Ա�֤�泯��)
// Back Face (Indices: 0,1,2, 0,2,3 relative to leg's first vertex)
8,  9, 10,     8, 10, 11,   // LF Back Face (Z=0.23)
// Front Face (Indices: 4,5,6, 4,6,7 relative to leg's first vertex)
12, 15, 14,    12, 14, 13,   // LF Front Face (Z=0.28), CCW: 4,7,6, 4,6,5 (FLL, FUL, FUR and FLL, FUR, FLR)
// Let's use the same pattern as the tabletop for consistency if possible
// Or a standard cube CCW: (FLL,FLR,FUR), (FLL,FUR,FUL) -> (12,13,14), (12,14,15)
// Standard CCW cube indices (vertex numbers are base + local_idx):
// Back face (local: 0,1,2, 0,2,3)
8,  9, 10,    8, 10, 11,
// Front face (local: 4,5,6, 4,6,7. Original: 4,5,6 means FLL,FLR,FUR)
12, 13, 14,   12, 14, 15,
// Left face (local: 0,3,7, 0,7,4)
8, 11, 15,    8, 15, 12,
// Right face (local: 1,2,6, 1,6,5)
9, 10, 14,    9, 14, 13,
// Bottom face (local: 0,1,5, 0,5,4)
8,  9, 13,    8, 13, 12,
// Top face (local: 3,2,6, 3,6,7)
11, 10, 14,   11, 14, 15,

// ����2: ��ǰ (RF) - �������� 16-23 (ƫ����Ϊ16)
16, 17, 18,   16, 18, 19, // Back
20, 21, 22,   20, 22, 23, // Front
16, 19, 23,   16, 23, 20, // Left
17, 18, 22,   17, 22, 21, // Right
16, 17, 21,   16, 21, 20, // Bottom
19, 18, 22,   19, 22, 23, // Top

// ����3: ��� (LB) - �������� 24-31 (ƫ����Ϊ24)
24, 25, 26,   24, 26, 27, // Back
28, 29, 30,   28, 30, 31, // Front
24, 27, 31,   24, 31, 28, // Left
25, 26, 30,   25, 30, 29, // Right
24, 25, 29,   24, 29, 28, // Bottom
27, 26, 30,   27, 30, 31, // Top

// ����4: �Һ� (RB) - �������� 32-39 (ƫ����Ϊ32)
32, 33, 34,   32, 34, 35, // Back
36, 37, 38,   36, 38, 39, // Front
32, 35, 39,   32, 39, 36, // Left
33, 37, 38,   33, 38, 34, // Right
32, 33, 37,   32, 37, 36, // Bottom
35, 34, 38,   35, 38, 39  // Top
}; // Total 180 indices: 36 for tabletop + 4 * 36 for legs.

Table::Table() : VAO(0), VBO(0), EBO(0) {}

Table::~Table() {
}

void Table::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tableIndices), tableIndices, GL_STATIC_DRAW);

    // λ������ (XYZ)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ��ɫ���� (RGB)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // ���VAO
}

// ע��: Shader& shader ͨ�����ã����⸴��Shader����
void Table::draw(Shader shader, const glm::mat4& modelMatrix) const {
    // shader.use(); // ȷ��shader��ʹ����, ����ⲿû���õĻ�
    shader.setMat4("model", modelMatrix);

    glBindVertexArray(VAO);
    // ���������� 180
    glDrawElements(GL_TRIANGLES, 180, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // ���ƺ�ͨ�����󣬵�ȡ���������Ⱦѭ���ṹ
}