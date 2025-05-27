#ifndef TABLE_H
#define TABLE_H
#include <vector>
#include "base/column.h"
#include "base/square.h"

class Table {
public:
	Table();
	void setup();
	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
	Column column_;
	glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.07f); // �����ⷴ�� (�Դ�ůɫ��)
	glm::vec3 kd_ = glm::vec3(0.7f, 0.6f, 0.4f);   // ������ (�׻�ɫ)
	glm::vec3 ks_ = glm::vec3(0.15f, 0.15f, 0.15f); // ���淴�� (�еȹ���)
	float ns_ = 30.0f;                             // �߹�ָ�� (���У�ģ��ľ�ذ���ש)
};

#endif