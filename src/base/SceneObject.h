#pragma once

#include <string>
#include <glm/glm.hpp>
#include "shader.h"

class SceneObject {
public:
    SceneObject(const std::string& name) : name_(name), position_(0.0f), scale_(1.0f) {}
    virtual ~SceneObject() = default;

    virtual void initialize() = 0;
    virtual void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) = 0;
    virtual void update(float deltaTime) = 0;

    const std::string& getName() const { return name_; }
    void setPosition(const glm::vec3& position) { position_ = position; }
    void setScale(float scale) { scale_ = scale; }

protected:
    std::string name_;
    glm::vec3 position_;
    float scale_;
}; 