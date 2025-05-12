#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
class DrawObject {
public:
	DrawObject(): vao_(0), vbo_(0), ebo_(0){}
	virtual ~DrawObject(){}

	// 初始化顶点数据和OpenGL缓冲区对象
	virtual void setup(){};

	// 绘制该对象
	// shaderProgramID: 当前使用的着色器程序ID
	// modelMatrix: 当前使用的模型矩阵
	virtual void draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const{};

protected:
	// 存储地面的 VAO, VBO, EBO 的 ID
	unsigned int vao_, vbo_, ebo_;
};
#endif // !DRAW_OBJECT_H
