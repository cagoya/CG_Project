#ifndef CHAIR_H
#define CHAIR_H
#include <vector>
#include "base/column.h"
#include "base/square.h"

class Chair {
public:
	Chair();
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	Column column_;
	glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.07f);
	glm::vec3 kd_ = glm::vec3(0.7f, 0.6f, 0.4f);
	glm::vec3 ks_ = glm::vec3(0.15f, 0.15f, 0.15f);
	float ns_ = 30.0f;                      
};

#endif