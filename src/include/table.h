#pragma once
#include <glm/glm.hpp>

// ����
class Table {
public:
    Table();
    ~Table();

    void setup();
    void draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const;

private:
    unsigned int VAO, VBO, EBO;
};
