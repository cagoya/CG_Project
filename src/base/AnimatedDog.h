#pragma once

#include "SceneObject.h"
#include <vector>
#include <memory>
#include "base/light.h"
// 向前声明 ObjectModel，避免在头文件中包含其完整定义
class ObjectModel;

class AnimatedDog : public SceneObject {
public:
    AnimatedDog(const std::string& name);
    ~AnimatedDog();

    // 初始化函数，接收一串模型文件路径
    void initialize(const std::vector<std::string>& framePaths, const std::string& mtlBasePath);
    
    void update(float deltaTime) override;
    
    // 我们需要重写 draw 函数来绘制正确的帧
    // 注意：这里的签名需要和你 SceneObject 基类中的 draw 虚函数保持一致
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) override;
    void initialize(const std::string& singlePath);

    // 设置动画播放速度
    void setFrameDuration(float duration) { frameDuration_ = duration; }

private:
    // 用一个 vector 存储每一帧的模型。使用智能指针来自动管理内存。
    std::vector<std::unique_ptr<ObjectModel>> animationFrames_;

    // 动画控制变量
    int currentFrame_ = 0;           // 当前播放到哪一帧
    float frameDuration_ = 0.1f;     // 每一帧的持续时间（秒），0.1f = 10 FPS
    float frameTimer_ = 0.0f;        // 当前帧的倒计时器
};