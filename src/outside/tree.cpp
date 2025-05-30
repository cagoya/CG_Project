#include "outside/tree.h"

Tree::Tree():tapering_(Tapering(0.15f,0.25f,20,glm::vec3(0.0f,0.0f,0.0f),"../../media/textures/tree.jpg")),
column_(Column(0.05f,0.5f,20,glm::vec3(0.0f,0.0f,0.0f),"../../media/textures/trunk.jpg")){}

void Tree::setup()
{
	tapering_.setup();
	column_.setup();
}

void Tree::draw(Shader& shader, const glm::mat4& modelMatrix) const
{
	column_.draw(shader, modelMatrix);
	float angle_degrees = 270.0f;
	float angle_radians = glm::radians(angle_degrees);
	glm::vec3 rotation_axis = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::rotate(modelMatrix, angle_radians, rotation_axis);
	tapering_.draw(shader, glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f)));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.6f));
	tapering_.draw(shader, glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f)));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.1f));
	tapering_.draw(shader, glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)));
}
