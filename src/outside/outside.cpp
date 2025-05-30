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

void Outside::draw(Shader& shader, const glm::mat4& modelMatrix)
{
    wall_.draw(shader, modelMatrix);
    ground_.draw(shader, modelMatrix);
    path_.draw(shader, modelMatrix);
    fence_.draw(shader, glm::mat4(1.0f));
    roof_.draw(shader, modelMatrix);
    door_.draw(shader, modelMatrix);
    stone_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f,0.01f,4.0f)));
    stone_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.01f, 4.0f)));
    tree_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f,0.0f,1.5f)));
    tree_.draw(shader, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 1.5f)));
    window_.draw(shader, modelMatrix);
}