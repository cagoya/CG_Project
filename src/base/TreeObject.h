#pragma once

#include "SceneObject.h"
#include "ObjectModel/ObjectModel.h"

class TreeObject : public SceneObject {
public:
    TreeObject(const std::string& name);
    ~TreeObject();

    void initialize(const std::string& normalMapPath) override;
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) override;
    void update(float deltaTime) override;

private:
    ObjectModel treeModel_;
}; 