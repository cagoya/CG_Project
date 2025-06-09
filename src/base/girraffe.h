//
// Created by Lenovo on 25-6-8.
//

#ifndef GIRRAFFE_H
#define GIRRAFFE_H
#include <memory>

#include "ObjectModel.h"
#include "SceneObject.h"


class girraffe: public SceneObject{
    public:
    girraffe();
    girraffe(const std::string& name, std::shared_ptr<girraffe>& self);
    void initialize(const std::string& normalMapPath) override;
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) override;
    void update(float deltaTime) override;
    private:
    ObjectModel model;
    glm::vec3 position;
    float scale;
};



#endif //GIRRAFFE_H
