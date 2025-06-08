#include "PoolObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PoolObject::PoolObject(const std::string& name) : SceneObject(name) {}

PoolObject::~PoolObject() {}

void PoolObject::initialize() {
    if (!pool_.initialize()) {
        std::cerr << "Failed to initialize swimming pool!" << std::endl;
    }
    pool_.setPosition(position_);
    pool_.setScale(scale_);
}

void PoolObject::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    // 更新游泳池的位置和缩放
    pool_.setPosition(position_);
    pool_.setScale(scale_);

    // 绘制游泳池
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position_);
    model = glm::scale(model, glm::vec3(scale_));
    
    pool_.draw(shader.getID(), model);
}

void PoolObject::update(float deltaTime) {
    // 可以在这里添加游泳池的动画或更新逻辑
} 