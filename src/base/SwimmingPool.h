#pragma once

#include "ObjectModel/ObjectModel.h"
#include <glm/glm.hpp>

class SwimmingPool {
public:
    SwimmingPool();
    ~SwimmingPool();

    // 初始化游泳池
    bool initialize();
    
    // 更新和绘制
    void update(float deltaTime);
    void draw(GLuint shaderProgram, const glm::mat4& model);
    
    // 设置游泳池的位置和大小
    void setPosition(const glm::vec3& position);
    void setScale(float scale);
    
    // 获取游泳池的位置和大小
    glm::vec3 getPosition() const { return position_; }
    float getScale() const { return scale_; }

private:
    ObjectModel* model_;  // 添加模型指针
    glm::vec3 position_ = glm::vec3(4.0f, 0.2f, 2.0f); // 默认位置
    float scale_ = 0.003f; // 默认缩放比例
    float time_ = 0.0f;    // 添加时间变量
}; 