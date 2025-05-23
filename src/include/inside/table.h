#pragma once
#include <glm/glm.hpp>
#include "base/draw_object.h"

// ����
class Table :DrawObject {
public:
    Table();
    ~Table();

    void setup();
    void draw(Shader shader, const glm::mat4& modelMatrix) const;

private:
    unsigned int VAO, VBO, EBO;
};
