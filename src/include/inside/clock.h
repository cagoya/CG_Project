#ifndef CLOCK_H
#define CLOCK_H
#include <vector>
#include "base/column.h"
#include "base/square.h"

class Clock {
public:
	Clock();
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix, bool useTexture) const;
private:
	Column column_;
	glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.07f);
	glm::vec3 kd_ = glm::vec3(0.7f, 0.6f, 0.4f);
	glm::vec3 ks_ = glm::vec3(0.9f, 0.7f, 0.0f);
	float ns_ = 80.0f;
};

#endif