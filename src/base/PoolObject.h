#pragma once

#include "SceneObject.h"
#include "SwimmingPool.h"

class PoolObject : public SceneObject {
public:
    PoolObject(const std::string& name);
    ~PoolObject();

    void initialize() override;
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) override;
    void update(float deltaTime) override;

private:
    SwimmingPool pool_;
}; 