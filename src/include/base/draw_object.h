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

	// ��ʼ���������ݺ�OpenGL����������
	virtual void setup(){};

	// ���Ƹö���
	// shaderProgramID: ��ǰʹ�õ���ɫ������ID
	virtual void draw(Shader& shader, const glm::mat4& modelMatrix) const{};

protected:
	// �洢����� VAO, VBO, EBO �� ID
	unsigned int vao_, vbo_, ebo_;
	//������Ϣ
	Material material_;
};
#endif // !DRAW_OBJECT_H
