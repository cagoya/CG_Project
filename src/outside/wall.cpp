#include "outside/wall.h" 
#include <glad/gl.h>
#include <iostream>

static const float WallVertices[] = {
    // --- ԭ�еĶ��� 0-29 (���ֲ���) ---
    // ���� (���� 0-3)
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,

    // ���� (���� 4-7) - ��Щԭʼ���涥����Ȼ�ǻ���
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f, // V4
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f, // V5
     0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // V6
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // V7

    // ���� (���� 8-11)
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    /*12 P_TF */    0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    /*13 P_TB */     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    /*14 P_BB */  0.5f,  0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
    /*15 P_BF */ 0.5f,  0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    // ����������� (�ݶ�����) (���� 16-18)
     -0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,
      0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,
      0.0f, 0.625f, 0.5f, 0.5f, 0.5f, 0.5f,

      // ����������� (�ݶ�����) (���� 19-21)
        -0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
         0.0f, 0.625f, -0.5f,0.5f, 0.5f, 0.5f,

         // ���� (���� 22-25)
          -0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
           0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
           0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
          -0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f,

          /*26*/ 0.5f, 0.175f, -0.15f,  0.5f, 0.5f, 0.5f,
          /*27*/ 0.5f, 0.175f,  0.15f,  0.5f, 0.5f, 0.5f,
          /*28*/ 0.5f, 0.325f,  0.15f,  0.5f, 0.5f, 0.5f,
          /*29*/ 0.5f, 0.325f, -0.15f,  0.5f, 0.5f, 0.5f,
          // --- END ԭ�еĶ��� 0-29 ---

          // --- �����Ķ��� (���� 30-35)������ Z=0.5f ƽ���� ---
    /*30 Hole_LB*/ -0.1f, 0.0f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*31 Hole_RB*/  0.1f, 0.0f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*32 Hole_RT*/  0.1f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*33 Hole_LT*/ -0.1f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*34 Aux_L  */ -0.5f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f, // ǽ��������ߣ��Ŷ�����ͬ�ߵ�
    /*35 Aux_R  */  0.5f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f  // ǽ�������ұߣ��Ŷ�����ͬ�ߵ�
    // --- END �����Ķ��� ---
};

// ǽ����������
static const unsigned int WallIndices[] = {
    // --- ������ԭʼ���� (����ԭ����ǽ������) ---
    // ���� (6 indices)
    0, 1, 2,  0, 2, 3,
    // ���� (6 indices)
    8, 9,10,  8,10,11,
    // �������� (24 indices)
    15, 27, 28, 15, 28, 12,
    12, 28, 29, 12, 29, 13,
    13, 29, 26, 13, 26, 14,
    14, 26, 27, 14, 27, 15,
    // ����������� (�ݶ���3 indices)
    16, 17, 18,
    // ����������� (�ݶ���3 indices)
    19, 20, 21,
    // ���� (6 indices)
    22, 23, 24,  22, 24, 25,
    // --- END ������ԭʼ���� ---

    // --- ���������������ڹ��ɴ��׶�������ǽ (18 ����) ---
    // ����ǽ��ԭʼ����: V4=(-0.5,0,0.5), V5=(0.5,0,0.5), V6=(0.5,0.5,0.5), V7=(-0.5,0.5,0.5)
    // �������Ŷ�����������: V30(Hole_LB), V31(Hole_RB), V32(Hole_RT), V33(Hole_LT), V34(Aux_L), V35(Aux_R)

    // �Ŷ���ߵĲ��� (����ǽ�����½� V4 ���Ŷ���࣬�����ϵ�ǽ�����Ͻ� V7)
    /* V4, V30(Hole_LB), V33(Hole_LT) */       4, 30, 33,
    /* V4, V33(Hole_LT), V34(Aux_L)   */       4, 33, 34,

    // �Ŷ��ұߵĲ��� (�����Ŷ��Ҳൽǽ�����½� V5�������ϵ�ǽ�����Ͻ� V6)
    /* V31(Hole_RB), V5, V35(Aux_R)   */       31, 5, 35,
    /* V31(Hole_RB), V35(Aux_R), V32(Hole_RT) */31, 35, 32,

    // �Ŷ��Ϸ��Ĳ��� (����ǽ�������������Ŷ��Ϸ�����)
    /* V34(Aux_L), V35(Aux_R), V6      */       34, 35, 6,
    /* V34(Aux_L), V6, V7              */       34, 6, 7
    // --- END ���������� ---
};

Wall::Wall() : DrawObject() {} 

Wall::~Wall() {}

void Wall::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(WallVertices), WallVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(WallIndices), WallIndices, GL_STATIC_DRAW);

    // λ������ (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ��ɫ���� (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Wall::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, sizeof(WallIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}