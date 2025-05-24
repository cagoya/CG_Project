#ifndef DOOR_H
#define DOOR_H

#include <glm/glm.hpp>
#include "base/square.h"
#include "base/draw_object.h"

class Door : DrawObject {
public:
    Door();
    ~Door() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
    glm::vec3 ka_ = glm::vec3(0.05f, 0.05f, 0.05f); // �����ⷴ��
    glm::vec3 kd_ = glm::vec3(0.8f, 0.8f, 0.8f);   // ������ (�ӽ���ɫ)
    glm::vec3 ks_ = glm::vec3(0.1f, 0.1f, 0.1f);   // ���淴�� (΢���Ĺ���)
    float ns_ = 10.0f;                             // �߹�ָ�� (��΢�Ĵֲڸ�)
};
#endif // !DOOR_H