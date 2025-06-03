#include "inside/inside.h"
#include "iostream"

void Inside::setup()
{
	floor_.setup();
	table_.setup();
	chair_.setup();
	clock_.setup();
	
}

void Inside::draw(Shader& shader, const glm::mat4& modelMatrix, std::string& sentence, std::string font, float font_size, int R, int G, int B,bool useTexture)
{
	table_.draw(shader, modelMatrix, useTexture);
	chair_.draw(shader, modelMatrix, useTexture);
	clock_.draw(shader, modelMatrix, useTexture);
	floor_.draw(shader, modelMatrix, useTexture);
	calligraphy_.generateTexture(sentence, font, font_size, R,G,B);
	calligraphy_.setup();
	calligraphy_.draw(shader, modelMatrix, useTexture);
}
void Inside::drawShadow(Shader& shader, const glm::mat4& modelMatrix, std::string& sentence, std::string font, float font_size, int R, int G, int B,bool useTexture)
{
	table_.draw(shader, modelMatrix, useTexture);
	chair_.draw(shader, modelMatrix, useTexture);
	clock_.draw(shader, modelMatrix, useTexture);
	floor_.draw(shader, modelMatrix, useTexture);
	calligraphy_.generateTexture(sentence, font, font_size, R,G,B);
	calligraphy_.setup();
	calligraphy_.draw(shader, modelMatrix, useTexture);
}
