#include "outside/outside.h"

void Outside::setup()
{
    wall_.setup();
    ground_.setup();
    path_.setup();
    fence_.setup();
    stone_.setup();
    tree_.setup();
    roof_.setup();
    door_.setup();
    window_.setup();
}

void Outside::draw(Shader& shader, const glm::mat4& modelMatrix, bool useTexture)
{
    wall_.draw(shader, modelMatrix,  useTexture);
    ground_.draw(shader, modelMatrix, useTexture);
    path_.draw(shader, modelMatrix, useTexture);
    fence_.draw(shader, glm::mat4(1.0f), useTexture);
    roof_.draw(shader, modelMatrix, useTexture);
    door_.draw(shader, modelMatrix, useTexture);
    stone_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f,0.01f,4.0f)), useTexture);
    stone_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.01f, 4.0f)), useTexture);
    tree_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f,0.0f,1.5f)), useTexture);
    tree_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 1.5f)), useTexture);
    window_.draw(shader, modelMatrix, useTexture);
}
void Outside::drawShadow(Shader& shader, const glm::mat4& modelMatrix, bool useTexture)
{
    wall_.draw(shader, modelMatrix,  useTexture);
    fence_.draw(shader, glm::mat4(1.0f),  useTexture);
    roof_.draw(shader, modelMatrix,  useTexture);
    door_.draw(shader, modelMatrix,  useTexture);
    stone_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f, 0.01f, 4.0f)), useTexture);
    stone_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.01f, 4.0f)), useTexture);
    tree_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 1.5f)), useTexture);
    tree_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 1.5f)), useTexture);
}