#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include<string>
#include "base/square.h"

// ��
class Sphere {
public:
	Sphere() = default;
	Sphere(float radius, int segments, const glm::vec3& color, std::string texture_path);
	~Sphere() = default;
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	void generateSphere(); // ����Բ�������Ķ������ݣ���� vertices_ �� indices_
	float radius_;
	int segments_;
	const glm::vec3 color_;
	std::string texture_path_;
};

#endif