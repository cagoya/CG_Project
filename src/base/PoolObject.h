#pragma once

#include "SceneObject.h"
#include "SwimmingPool.h"
#include "base/shader.h"

class PoolObject : public SceneObject {
public:
    PoolObject(const std::string& name);
    ~PoolObject();

    void initialize(const std::string& normalMapPath) override;
    void update(float deltaTime) override;

    // 1. 我们自己新增的、功能更强大的 draw 函数
    void draw(const Shader& defaultShader, const Shader& waterShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight);

    // 2. 为了满足基类要求而实现的 draw 函数，并用 override 明确指出
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) override;

private:
    SwimmingPool pool_;
};