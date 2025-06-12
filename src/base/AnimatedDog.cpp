#include "AnimatedDog.h"
#include "ObjectModel/ObjectModel.h"
#include "base/shader.h"
#include "base/light.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

AnimatedDog::AnimatedDog(const std::string& name) : SceneObject(name) {}

AnimatedDog::~AnimatedDog() {}

void AnimatedDog::initialize(const std::vector<std::string>& framePaths, const std::string& mtlBasePath) {
    std::cout << "Initializing AnimatedDog with " << framePaths.size() << " frames." << std::endl;
    for (const auto& path : framePaths) {
        auto frameModel = std::make_unique<ObjectModel>();
        if (frameModel->load(path, mtlBasePath)) {
            animationFrames_.push_back(std::move(frameModel));
        } else {
            std::cerr << "Failed to load dog animation frame: " << path << std::endl;
        }
    }

    animationRanges_[DogState::IDLE]       =  {32, 63};
    animationRanges_[DogState::SIT]        = {64, 95};
    animationRanges_[DogState::JUMP_AIR]   = {96, 127};
    animationRanges_[DogState::WALK]       = {128, 159};
    animationRanges_[DogState::RUN]        = {160, 191};
    animationRanges_[DogState::JUMP_START] = {192, 224};
    animationRanges_[DogState::BARK]       = {225, 249};
    setState(DogState::IDLE);
}

void AnimatedDog::jump() {
    if (isOnGround_) {
        setState(DogState::JUMP_START);
        isOnGround_ = false;
        verticalVelocity_ = jumpStrength_;
    }
}

void AnimatedDog::setState(DogState newState) {
    // 只有当状态真的改变时，才重置动画
    if (currentState_ != newState) {
        currentState_ = newState;
        currentFrame_ = animationRanges_[currentState_].first;
        frameTimer_ = frameDuration_;
    }
}

void AnimatedDog::update(float deltaTime) {
    if (animationFrames_.empty()) return;

    if (!isOnGround_) {
        verticalVelocity_ -= gravity_ * deltaTime;
        position_.y += verticalVelocity_ * deltaTime;
        // 检查是否落地
        if (position_.y <= groundHeight_) {
            position_.y = groundHeight_;
            verticalVelocity_ = 0.0f;
            isOnGround_ = true;
            // IDLE 或 WALK
            setState(DogState::IDLE);
        }
    }

    frameTimer_ -= deltaTime;
    if (frameTimer_ <= 0.0f) {
        frameTimer_ = frameDuration_;
        currentFrame_++;
        const auto& range = animationRanges_[currentState_];
        int startFrame = range.first;
        int endFrame = range.second;
        if (currentState_ == DogState::JUMP_START && currentFrame_ > endFrame) {
            // 如果起跳动画播放完毕，且小狗仍在空中，则无缝切换到滞空动画
            setState(DogState::JUMP_AIR);
        }
        // 循环播放当前状态的动画
        else if (currentFrame_ > endFrame || currentFrame_ < startFrame) {
            currentFrame_ = startFrame;
        }
    }
}
void AnimatedDog::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection, const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    if (animationFrames_.empty() || currentFrame_ >= animationFrames_.size() || !animationFrames_[currentFrame_]) {
        return;
    }

    shader.use();

    // 设置通用的 uniforms
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // 设置光照 (你需要确保你的 objModelShader 中有这些 uniforms)
    // shader.setVec3("viewPos", cameraPosition); // 假设 cameraPosition 是一个可以访问的全局变量或传入参数
    shader.setVec3("ambientLight.color", ambientLight.color);
    shader.setFloat("ambientLight.intensity", ambientLight.intensity);
    shader.setVec3("directionalLight.direction", directionalLight.direction);
    shader.setVec3("directionalLight.color", directionalLight.color);
    shader.setFloat("directionalLight.intensity", directionalLight.intensity);
    // ... 其他光照 ...

    // --- 计算并设置此对象的模型矩阵 (包含旋转) ---
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::rotate(modelMatrix, rotationY, glm::vec3(0.0f, 1.0f, 0.0f)); // 沿Y轴旋转
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale_));
    shader.setMat4("model", modelMatrix);

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    shader.setMat3("normalMatrix", normalMatrix);

    // 获取当前要绘制的模型帧
    ObjectModel* currentModel = animationFrames_[currentFrame_].get();

    // 遍历当前帧模型的所有子网格并绘制
    for (const auto& mesh : currentModel->getSubMeshes()) {
        glActiveTexture(GL_TEXTURE0);
        shader.setInt("material.diffuse", 0);
        glBindTexture(GL_TEXTURE_2D, mesh.diffuseTextureId);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void AnimatedDog::setFrameDuration(float duration) {
    frameDuration_ = duration;
    // 如果持续时间小于0，则冻结动画
    if (frameDuration_ < 0.0f) frameDuration_ = 99999.0f;
}
void AnimatedDog::initialize(const std::string& singlePath) {
    // 这个函数是为了满足基类的要求。
    std::cerr << "Warning: AnimatedDog's generic initialize(string) was called, but it does nothing." << std::endl;
}
bool AnimatedDog::isOnGround()
{
    return isOnGround_;
}