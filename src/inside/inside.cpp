#include "inside/inside.h"

void Inside::setup()
{
	floor_.setup();
	table_.setup();
	chair_.setup();
	clock_.setup();
	
}

void Inside::draw(Shader& shader, const glm::mat4& modelMatrix, std::string& sentence)
{
	table_.draw(shader, modelMatrix);
	chair_.draw(shader, modelMatrix);
	clock_.draw(shader, modelMatrix);
	floor_.draw(shader, modelMatrix);
	calligraphy_.generateTexture(sentence);
	calligraphy_.setup();
	calligraphy_.draw(shader, modelMatrix);
}
