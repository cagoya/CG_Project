#ifndef COLUMN_H
#define COLUMN_H

#include <glm/glm.hpp>
#include<string>
#include "base/square.h"

// Բ����
class Column{
public:
	Column() = default;
	Column(float radius, float height, int segments, const glm::vec3& color, std::string texture_path);
	~Column() = default;
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	void generateColumnSideData(); // ����Բ�������Ķ������ݣ���� vertices_ �� indices_
	float radius_;
	float height_;
	int segments_;
	const glm::vec3 color_;
};


#endif 
