#ifndef SQUARE_H
#define SQUARE_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include<vector>
#include<string>

#include "base/draw_object.h"

// 长方形，其实不止长方形，任何图形都可以画，只要指定好顶点坐标和索引
class Square : DrawObject {
public:
	Square() = default;
	Square(std::vector<float> vertices, std::vector<unsigned int> indices);
	~Square();
	void setup();

	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
	inline std::vector<float>& getVertices() { return vertices_; };
	inline std::vector<unsigned int>& getIndices() { return indices_; };
	inline std::string& getTexturePath() { return texture_path_; }
private:
	GLuint textureID_ = 0;
	std::vector<float> vertices_;
	std::vector<unsigned int> indices_;
	std::string texture_path_;
};

#endif //SQUARE_H
