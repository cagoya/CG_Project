#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include "base/tapering.h"
#include "base/column.h"
#include "base/draw_object.h"

class Tree : DrawObject {
public:
    Tree();
    ~Tree() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix, bool useTexture) const;
private:
    Tapering tapering_;
    Column column_;
    glm::vec3 ka_ = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 kd_ = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 ks_ = glm::vec3(0.05f, 0.05f, 0.05f);
    float ns_ = 10.0f;
};
#endif // !Tree_H