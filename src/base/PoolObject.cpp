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

// +++ 新增：为了满足基类要求而添加的 draw 函数实现 +++
void PoolObject::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    // 这是一个“备用”的绘制方法。
    // 当它被调用时，我们让它调用新的、功能更强的 draw 函数，
    // 并且两个着色器参数都传递同一个。
    // 这样可以保证即使通过基类指针调用，它也能被绘制出来（虽然水面没有特殊效果）。
    // 注意：我们假设相机位置在原点，或者你需要从某个地方获取它。
    this->draw(shader, shader, view, projection, glm::vec3(0.0f),ambientLight, directionalLight, spotLight);
}

void PoolObject::update(float deltaTime) {
    pool_.update(deltaTime);
}