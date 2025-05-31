#include "inside/inside.h"
#include "iostream"

void Inside::setup()
{
	floor_.setup();
	table_.setup();
	chair_.setup();
	clock_.setup();
	
}

void Inside::draw(Shader& shader, const glm::mat4& modelMatrix, std::string& sentence, std::string font, float font_size, int R, int G, int B)
{
	table_.draw(shader, modelMatrix);
	chair_.draw(shader, modelMatrix);
	clock_.draw(shader, modelMatrix);
	floor_.draw(shader, modelMatrix);
	calligraphy_.generateTexture(sentence, font, font_size, R,G,B);
	calligraphy_.setup();
	calligraphy_.draw(shader, modelMatrix);
}
