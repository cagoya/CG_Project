#include "floor.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

static const float floorVertices[] = {
    -0.5f, 0.01f, -0.5f,  0.0f, 0.0f, 0.0f,
     0.5f, 0.01f, -0.5f,  0.0f, 0.0f, 0.0f,
     0.5f, 0.01f,  0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f, 0.01f,  0.5f,  0.0f, 0.0f, 0.0f,
};

static const unsigned int floorIndices[] = {
    0, 1, 2,
    0, 2, 3
};

Floor::Floor() : DrawObject() {}

Floor::~Floor() {
    if (vao_) glDeleteVertexArrays(1, &vao_);
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (ebo_) glDeleteBuffers(1, &ebo_);
}

void Floor::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    // Î»ÖÃ
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ÑÕÉ«
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Floor::draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const {
    unsigned int modelLoc = glGetUniformLocation(shaderProgramID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
