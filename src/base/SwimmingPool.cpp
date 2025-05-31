#include "SwimmingPool.h"
#include <iostream>

SwimmingPool::SwimmingPool() : model_(nullptr), time_(0.0f) {}

SwimmingPool::~SwimmingPool() {
    if (model_) {
        delete model_;
        model_ = nullptr;
    }
}

bool SwimmingPool::initialize() {
    model_ = new ObjectModel();
    std::string objPath = "../../media/model/new_pool/source/swimmingPool.obj";
    std::string mtlPath = "../../media/model/new_pool/source/";
    std::cout << "Attempting to load swimming pool from: " << objPath << std::endl;
    if (!model_->load(objPath, mtlPath)) {
        std::cerr << "Failed to load swimming pool model!" << std::endl;
        delete model_;
        model_ = nullptr;
        return false;
    }
    std::cout << "Successfully loaded swimming pool model!" << std::endl;
    return true;
}

void SwimmingPool::update(float deltaTime) {
    time_ += deltaTime;
    // 添加调试输出
    static float lastDebugTime = 0.0f;
    if (time_ - lastDebugTime > 1.0f) { // 每秒输出一次
        std::cout << "Current time: " << time_ << std::endl;
        lastDebugTime = time_;
    }
}

void SwimmingPool::draw(GLuint shaderProgram, const glm::mat4& model) {
    if (!model_) return;

    // 设置时间uniform
    GLint timeLocation = glGetUniformLocation(shaderProgram, "time");
    if (timeLocation != -1) {
        glUniform1f(timeLocation, time_);
        // 添加调试输出
        static float lastDebugTime = 0.0f;
        if (time_ - lastDebugTime > 1.0f) { // 每秒输出一次
            std::cout << "Setting time uniform: " << time_ << std::endl;
            lastDebugTime = time_;
        }
    } else {
        std::cerr << "Failed to get time uniform location!" << std::endl;
    }

    model_->draw(shaderProgram, model);
}

void SwimmingPool::setPosition(const glm::vec3& position) {
    position_ = position;
}

void SwimmingPool::setScale(float scale) {
    scale_ = scale;
}