#include "TreeObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TreeObject::TreeObject(const std::string& name) : SceneObject(name) {}

TreeObject::~TreeObject() {}

void TreeObject::initialize(const std::string& normalMapPath) {
    // 修改模型加载路径,确保MTL文件路径正确
    if (!treeModel_.load("../../media/model/tree/woodSpring.obj", "../../media/model/tree")) {
        std::cerr << "Failed to initialize tree model!" << std::endl;
    }
}

void TreeObject::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    // 创建模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position_);
    model = glm::scale(model, glm::vec3(scale_));
    
    // 绘制树
    treeModel_.draw(shader.getID(), model);
}

void TreeObject::update(float deltaTime) {
    // 可以在这里添加树的动画或更新逻辑
} 