#ifndef FENCE_H
#define FENCE_H
#include "base/square.h"

class Fence {
public:
	Fence();
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.07f);
	glm::vec3 kd_ = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 ks_ = glm::vec3(0.8f, 0.8f, 0.8f);
	float ns_ = 150.0f;
};

#endif // !FENCE_H
