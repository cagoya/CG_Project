#include "outside/wall.h" 
#include <glad/gl.h>
#include <iostream>

static const float WallVertices[] = {
    // --- 原有的顶点 0-29 (保持不变) ---
    // 背面 (索引 0-3)
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,

    // 正面 (索引 4-7) - 这些原始正面顶点仍然是基础
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f, // V4
     0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f, // V5
     0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // V6
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // V7

    // 左面 (索引 8-11)
    -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f, 0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    /*12 P_TF */    0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    /*13 P_TB */     0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    /*14 P_BB */  0.5f,  0.0f, -0.5f,  0.5f, 0.5f, 0.5f,
    /*15 P_BF */ 0.5f,  0.0f,  0.5f,  0.5f, 0.5f, 0.5f,

    // 正面的三角形 (屋顶部分) (索引 16-18)
     -0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,
      0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,
      0.0f, 0.625f, 0.5f, 0.5f, 0.5f, 0.5f,

      // 背面的三角形 (屋顶部分) (索引 19-21)
        -0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
         0.0f, 0.625f, -0.5f,0.5f, 0.5f, 0.5f,

         // 顶面 (索引 22-25)
          -0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
           0.5f, 0.5f, -0.5f,  0.6f, 0.6f, 0.6f,
           0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
          -0.5f, 0.5f,  0.5f,  0.6f, 0.6f, 0.6f,

          /*26*/ 0.5f, 0.175f, -0.15f,  0.5f, 0.5f, 0.5f,
          /*27*/ 0.5f, 0.175f,  0.15f,  0.5f, 0.5f, 0.5f,
          /*28*/ 0.5f, 0.325f,  0.15f,  0.5f, 0.5f, 0.5f,
          /*29*/ 0.5f, 0.325f, -0.15f,  0.5f, 0.5f, 0.5f,
          // --- END 原有的顶点 0-29 ---

          // --- 新增的顶点 (索引 30-35)，都在 Z=0.5f 平面上 ---
    /*30 Hole_LB*/ -0.1f, 0.0f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*31 Hole_RB*/  0.1f, 0.0f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*32 Hole_RT*/  0.1f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*33 Hole_LT*/ -0.1f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f,
    /*34 Aux_L  */ -0.5f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f, // 墙体正面左边，门洞顶部同高点
    /*35 Aux_R  */  0.5f, 0.3f,  0.5f,   0.5f, 0.5f, 0.5f  // 墙体正面右边，门洞顶部同高点
    // --- END 新增的顶点 ---
};

// 墙体索引数据
static const unsigned int WallIndices[] = {
    // --- 保留的原始索引 (除了原正面墙的索引) ---
    // 背面 (6 indices)
    0, 1, 2,  0, 2, 3,
    // 左面 (6 indices)
    8, 9,10,  8,10,11,
    // 右面特征 (24 indices)
    15, 27, 28, 15, 28, 12,
    12, 28, 29, 12, 29, 13,
    13, 29, 26, 13, 26, 14,
    14, 26, 27, 14, 27, 15,
    // 正面的三角形 (屋顶，3 indices)
    16, 17, 18,
    // 背面的三角形 (屋顶，3 indices)
    19, 20, 21,
    // 顶面 (6 indices)
    22, 23, 24,  22, 24, 25,
    // --- END 保留的原始索引 ---

    // --- 新增的索引，用于构成带孔洞的正面墙 (18 索引) ---
    // 正面墙的原始顶点: V4=(-0.5,0,0.5), V5=(0.5,0,0.5), V6=(0.5,0.5,0.5), V7=(-0.5,0.5,0.5)
    // 新增的门洞及辅助顶点: V30(Hole_LB), V31(Hole_RB), V32(Hole_RT), V33(Hole_LT), V34(Aux_L), V35(Aux_R)

    // 门洞左边的部分 (连接墙体左下角 V4 到门洞左侧，并向上到墙体左上角 V7)
    /* V4, V30(Hole_LB), V33(Hole_LT) */       4, 30, 33,
    /* V4, V33(Hole_LT), V34(Aux_L)   */       4, 33, 34,

    // 门洞右边的部分 (连接门洞右侧到墙体右下角 V5，并向上到墙体右上角 V6)
    /* V31(Hole_RB), V5, V35(Aux_R)   */       31, 5, 35,
    /* V31(Hole_RB), V35(Aux_R), V32(Hole_RT) */31, 35, 32,

    // 门洞上方的部分 (连接墙体左右两侧在门洞上方区域)
    /* V34(Aux_L), V35(Aux_R), V6      */       34, 35, 6,
    /* V34(Aux_L), V6, V7              */       34, 6, 7
    // --- END 新增的索引 ---
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

    // 位置属性 (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 颜色属性 (location = 1)
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