#ifndef TAPERING_H
#define TAPERING_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include<vector>
#include"base/shader.h"
#include<string>

#include"base/square.h"

// Բ׶
class Tapering {
public:
	Tapering() = default;
	Tapering(float radius, float height, int segments, const glm::vec3& color, std::string texture_path);
	~Tapering() = default;
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	void generateTapering();
	float radius_;
	float height_;
	int segments_;
	const glm::vec3 color_;
	std::string texture_path_;
};
#endif // !TAPERING_H
