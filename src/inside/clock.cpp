#include "inside/clock.h"

Clock::Clock() : column_(Column(0.1f, 0.01f, 20, glm::vec3(0.0f, 0.0f, 0.0f), "../../media/textures/clock.jpg")){}


void Clock::setup()
{
	column_.setup();
}

void Clock::draw(Shader& shader, const glm::mat4& modelMatrix,bool useTexture) const
{
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    float angle_degrees = 90.0f;
    float angle_radians = glm::radians(angle_degrees);
    glm::vec3 rotation_axis1 = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 model = glm::rotate(modelMatrix, angle_radians, rotation_axis1);
    glm::vec3 rotation_axis2 = glm::vec3(0.0f, 1.0f, 0.0f);
    model = glm::rotate(model, angle_radians, rotation_axis2);
    column_.draw(shader, glm::translate(model, glm::vec3(0.0f,0.95f, 0.7f)), useTexture);
}
