//
// Created by Lenovo on 25-6-8.
//

#include "girraffe.h"

#include <memory>
#include <glm/ext/matrix_transform.hpp>

girraffe::girraffe(): SceneObject("girraffe")
{
    std::string objPath = "../../media/model/giraffe.obj";
    position = glm::vec3(0.0f);
    scale = 1.0f;
}

girraffe::girraffe(const std::string& name, std::shared_ptr<girraffe>& self): SceneObject(name)
{
    std::string objPath = "../../media/model/giraffe.obj";
    position = glm::vec3(0.0f);
    scale = 1.0f;
}

void girraffe::initialize(const std::string& normalMapPath) {
    if (!model.load("../../media/model/giraffe.obj", "../../media/model")) {
        std::cerr << "Failed to initialize giraffe model!" << std::endl;
    }
}

void girraffe::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    model.draw(shader.id_, modelMatrix);
}

void girraffe::update(float deltaTime) {
    // 实现更新逻辑
}