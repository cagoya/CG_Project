#pragma once

#include "SceneObject.h"
#include <vector>
#include <memory>
#include <map>
#include <string>

class ObjectModel;

enum class DogState {
    IDLE,       // 静止
    SIT,        // 坐下
    JUMP_AIR,   // 滞空
    WALK,       // 行走
    RUN,        // 跑步
    JUMP_START, // 起跳
    BARK        // 汪汪叫
};

class AnimatedDog : public SceneObject {
public:
    AnimatedDog(const std::string& name);
    ~AnimatedDog();
    // 初始化函数，现在会记录每个动画片段的帧范围
    void initialize(const std::vector<std::string>& framePaths, const std::string& mtlBasePath);
    // Update 函数现在会根据当前状态来更新动画帧
    void update(float deltaTime) override;
    // Draw 函数现在会使用小狗的朝向来正确绘制
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection, const AmbientLight&, const DirectionalLight&, const SpotLight&) override;
    // 公共接口
    void setFrameDuration(float duration);
    void initialize(const std::string& singlePath);
    void setState(DogState newState);
    void setPosition(const glm::vec3& position) { position_ = position; }
    glm::vec3 getPosition() const { return position_; }
    void setScale(float scale) { scale_ = scale; }
    bool isOnGround();
    // 小狗的朝向（以弧度表示，Y轴旋转）
    float rotationY = 0.0f;
    float runSpeed = 2.0f;
    float walkSpeed = 0.8f;
    void jump();
    DogState getCurrentState() { return currentState_; }
private:
    // 存储每一帧
    std::vector<std::unique_ptr<ObjectModel>> animationFrames_;
    glm::vec3 position_;
    // 状态机核心：一个映射表，存储每种状态对应的动画起始帧和结束帧
    std::map<DogState, std::pair<int, int>> animationRanges_;
    DogState currentState_ = DogState::IDLE;
    // 动画播放相关
    int currentFrame_ = 0;
    float frameDuration_ = 0.05f;
    float frameTimer_ = 0.0f;

    float verticalVelocity_ = 0.0f; // 垂直速度
    bool isOnGround_ = true;       // 是否在地面上
    const float gravity_ = 5.8f;     // 重力加速度
    const float jumpStrength_ = 4.0f;  // 跳跃力度
    const float groundHeight_ = 0.05f; // 地面高度
};
