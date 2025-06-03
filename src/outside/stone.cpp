#include "outside/stone.h"

Stone::Stone():column_(Column(0.15f,0.02f,20,glm::vec3(0.0f,0.0f,0.0f),"../../media/textures/stone.jpg")),
sphere_(Sphere(0.15f, 20,glm::vec3(0.0f,0.0f,0.0f),"../../media/textures/stone.jpg")){}

void Stone::setup()
{
	column_.setup();
	sphere_.setup();
}

void Stone::draw(Shader& shader, const glm::mat4& modelMatrix, bool useTexture) const
{
	column_.draw(shader, modelMatrix,useTexture);
	sphere_.draw(shader, glm::translate(modelMatrix, glm::vec3(0.0f,0.13f,0.0f)), useTexture);
}
