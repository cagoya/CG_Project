#ifndef CALLIGRAPHY_H
#define CALLIGRAPHY_H
#include "base/square.h"

class Calligraphy {
public:
	Calligraphy();
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
	// generate the texture for character
	void generateTexture(std::string sentence);
private:
	Square square_;
	Square char_square_;
	glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.07f);
	glm::vec3 kd_ = glm::vec3(0.7f, 0.6f, 0.4f);
	glm::vec3 ks_ = glm::vec3(0.01f, 0.01f, 0.01f);
	float ns_ = 5.0f;
};

#endif