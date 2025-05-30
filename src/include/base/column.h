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
	inline float& getRadius() { return radius_; }
	inline float& getHeight() { return height_; }
	inline int& getSegments() { return segments_; }
	inline glm::vec3& getColor(){ return color_; }
private:
	Square square_;
	void generateColumnSideData(); // 生成圆柱体侧面的顶点数据，填充 vertices_ 和 indices_
	float radius_;
	float height_;
	int segments_;
	glm::vec3 color_;
};


#endif 
