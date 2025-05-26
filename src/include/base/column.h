#ifndef COLUMN_H
#define COLUMN_H

#include <glm/glm.hpp>
#include<string>
#include "base/square.h"

// 圆柱体
class Column{
public:
	Column() = default;
	Column(float radius, float height, int segments, const glm::vec3& color, std::string texture_path);
	~Column() = default;
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	void generateColumnSideData(); // 生成圆柱体侧面的顶点数据，填充 vertices_ 和 indices_
	float radius_;
	float height_;
	int segments_;
	const glm::vec3 color_;
};


#endif 
