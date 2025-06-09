#ifndef STONE_H
#define STONE_H


#include <glm/glm.hpp>

#include "base/draw_object.h"
#include "base/column.h"
#include "base/sphere.h"

class Stone : DrawObject {
public:
    Stone();
    ~Stone() = default;

    void setup();

    void draw(Shader& shader, const glm::mat4& modelMatrix, bool useTexture) const;
private:
    Column column_;
    Sphere sphere_;
    glm::vec3 ka_ = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 kd_ = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 ks_ = glm::vec3(0.1f, 0.1f, 0.1f);
    float ns_ = 30.0f;
};

#endif