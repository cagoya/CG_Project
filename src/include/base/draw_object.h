#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H

#include"base/shader.h"
#include"base/light.h"

class DrawObject {
public:
	DrawObject(): vao_(0), vbo_(0), ebo_(0){}
	virtual ~DrawObject()
	{
		if (vao_) {
			glDeleteVertexArrays(1, &vao_);
		}
		if (vbo_) {
			glDeleteBuffers(1, &vbo_);
		}
		if (ebo_)
		{
			glDeleteBuffers(1, &ebo_);
		}
	}

	// 初始化顶点数据和OpenGL缓冲区对象
	virtual void setup(){};

	// 绘制该对象
	// shaderProgramID: 当前使用的着色器程序ID
	virtual void draw(Shader& shader, const glm::mat4& modelMatrix) const{};

protected:
	// 存储地面的 VAO, VBO, EBO 的 ID
	unsigned int vao_, vbo_, ebo_;
	//材质信息
	Material material_;
};
#endif // !DRAW_OBJECT_H
