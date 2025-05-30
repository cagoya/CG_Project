#ifndef PATH_H
#define PATH_H
#include "base/square.h"

class Path {
public:
	Path();
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	glm::vec3 ka_ = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 kd_ = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 ks_ = glm::vec3(0.01f, 0.01f, 0.01f);
	float ns_ = 15.0f;
};

#endif