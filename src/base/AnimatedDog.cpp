#include "AnimatedDog.h"
#include "ObjectModel/ObjectModel.h" // 在 .cpp 文件中包含完整的定义
#include "base/shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

AnimatedDog::AnimatedDog(const std::string& name) : SceneObject(name) {}

// 析构函数会自动处理 unique_ptr，无需手动 delete
AnimatedDog::~AnimatedDog() {}

void AnimatedDog::initialize(const std::vector<std::string>& framePaths, const std::string& mtlBasePath) {
    std::cout << "Initializing AnimatedDog with " << framePaths.size() << " frames." << std::endl;
    for (const auto& path : framePaths) {
        //为每一帧都创建一个新的 ObjectModel 实例
        auto frameModel = std::make_unique<ObjectModel>();
        if (frameModel->load(path, mtlBasePath)) {
            animationFrames_.push_back(std::move(frameModel));
        } else {
            std::cerr << "Failed to load dog animation frame: " << path << std::endl;
        }
    }
    frameTimer_ = frameDuration_;
}

void AnimatedDog::update(float deltaTime) {
    // 如果没有动画帧，则不执行任何操作
    if (animationFrames_.empty()) {
        return;
    }

    // 更新当前帧的计时器
    frameTimer_ -= deltaTime;

    // 如果计时器结束，则切换到下一帧
    if (frameTimer_ <= 0.0f) {
        currentFrame_++;
        // 如果播放到最后一帧，则循环回第一帧
        if (currentFrame_ >= animationFrames_.size()) {
            currentFrame_ = 0;
        }
        // 重置计时器
        frameTimer_ = frameDuration_;
    }
}

// 在 AnimatedDog.cpp 中
// 在 AnimatedDog.cpp 中

void AnimatedDog::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection, const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    // 安全检查，确保我们有可以绘制的帧
    if (animationFrames_.empty() || currentFrame_ >= animationFrames_.size() || !animationFrames_[currentFrame_]) {
        return;
    }

    // 激活要使用的着色器程序 (defaultShader, 也就是 objModelShader)
    shader.use();

    // --- 设置所有对象共用的 Uniforms (由 SceneManager 传入) ---
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    // shader.setVec3("viewPos", cameraPosition); // 假设 cameraPosition 可以从某处获得，如果不行，暂时注释掉

    // 设置光照 (变量名需与 objModelShader 中的定义完全一致)
    // 假设你的 objModelShader 使用 "ambientLight", "directionalLight" 等名字
    shader.setVec3("ambientLight.color", ambientLight.color);
    shader.setFloat("ambientLight.intensity", ambientLight.intensity);
    shader.setVec3("directionalLight.direction", directionalLight.direction);
    shader.setVec3("directionalLight.color", directionalLight.color);
    shader.setFloat("directionalLight.intensity", directionalLight.intensity);
    shader.setVec3("spotLight.position", spotLight.position);
    shader.setVec3("spotLight.direction", spotLight.direction);
    // ... 设置所有需要的光照参数 ...

    // --- 设置此对象独有的 Uniforms ---
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale_));
    shader.setMat4("model", modelMatrix);

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    shader.setMat3("normalMatrix", normalMatrix);


    // --- 获取当前动画帧的模型并绘制 ---
    ObjectModel* currentModel = animationFrames_[currentFrame_].get();

    // +++ 新增的关键部分：遍历当前帧模型的每一个子网格并绘制 +++
    for (const auto& mesh : currentModel->getSubMeshes()) {
        // 1. 绑定这个子网格的纹理到纹理单元 0
        glActiveTexture(GL_TEXTURE0);
        // 你的 objModelShader 期望的纹理采样器名叫 "material.diffuse"
        shader.setInt("material.diffuse", 0);
        glBindTexture(GL_TEXTURE_2D, mesh.diffuseTextureId);

        // 2. 绑定这个子网格的VAO并发出绘制命令
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0); // 全部绘制完毕后解绑
}
void AnimatedDog::initialize(const std::string& singlePath) {
    // 这个函数是为了满足基类的要求。
    // 我们可以让它什么都不做，或者打印一个警告，防止被意外调用。
    std::cerr << "Warning: AnimatedDog's generic initialize(string) was called. "
              << "You should use initialize(vector<string>, ...) to load animation frames." << std::endl;

    // 如果你希望它加载一个静态模型作为默认，也可以在这里实现。
    // 为简单起见，我们暂时让它为空。
}