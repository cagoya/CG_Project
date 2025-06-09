#pragma once

#include "ObjectModel/ObjectModel.h"
#include "base/shader.h"
#include <glm/glm.hpp>
#include <string>
#include <base/light.h>

class SwimmingPool {
public:
    SwimmingPool();
    ~SwimmingPool();

    bool initialize(const std::string& normalMapPath);
    void update(float deltaTime);
    void draw(const Shader& defaultShader, const Shader& waterShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition, const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight);

    void setPosition(const glm::vec3& position);
    void setScale(float scale);

    glm::vec3 getPosition() const { return position_; }
    float getScale() const { return scale_; }

private:
    ObjectModel* model_ = nullptr;
    glm::vec3 position_;
    float scale_;
    float time_ = 0.0f;
    GLuint waterNormalMapID_ = 0;
};
