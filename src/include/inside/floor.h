#ifndef FLOOR_H
#define FLOOR_H

#include <glm/glm.hpp>
#include "base/square.h"
#include "base/draw_object.h"

//���ڵذ�
class Floor : DrawObject {
public:
    Floor();
    ~Floor() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
    glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.07f); // �����ⷴ�� (�Դ�ůɫ��)
    glm::vec3 kd_ = glm::vec3(0.7f, 0.6f, 0.4f);   // ������ (�׻�ɫ)
    glm::vec3 ks_ = glm::vec3(0.15f, 0.15f, 0.15f); // ���淴�� (�еȹ���)
    float ns_ = 30.0f;                             // �߹�ָ�� (���У�ģ��ľ�ذ���ש)
};
#endif // !FLOOR_H