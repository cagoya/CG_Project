#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
class DrawObject {
public:
	DrawObject(): vao_(0), vbo_(0), ebo_(0){}
	virtual ~DrawObject(){}

	// ��ʼ���������ݺ�OpenGL����������
	virtual void setup(){};

	// ���Ƹö���
	// shaderProgramID: ��ǰʹ�õ���ɫ������ID
	// modelMatrix: ��ǰʹ�õ�ģ�;���
	virtual void draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const{};

protected:
	// �洢����� VAO, VBO, EBO �� ID
	unsigned int vao_, vbo_, ebo_;
};
#endif // !DRAW_OBJECT_H
