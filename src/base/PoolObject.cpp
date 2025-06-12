#include "PoolObject.h"
#include "base/shader.h"
#include <glm/gtc/matrix_transform.hpp>

PoolObject::PoolObject(const std::string& name) : SceneObject(name) {}

PoolObject::~PoolObject() {}

void PoolObject::initialize(const std::string& normalMapPath) {
    if (!pool_.initialize(normalMapPath)) {
        std::cerr << "Failed to initialize swimming pool from PoolObject!" << std::endl;
    }
}

// 新的 draw 函数的实现
void PoolObject::draw(const Shader& defaultShader, const Shader& waterShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    pool_.setPosition(this->position_);
    pool_.setScale(this->scale_);
    pool_.draw(defaultShader, waterShader, view, projection, cameraPosition, ambientLight, directionalLight, spotLight);
}

void PoolObject::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    this->draw(shader, shader, view, projection, glm::vec3(0.0f),ambientLight, directionalLight, spotLight);
}

void PoolObject::update(float deltaTime) {
    pool_.update(deltaTime);
}