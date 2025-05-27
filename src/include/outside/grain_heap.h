#ifndef GRAIN_H
#define GRAIN_H

#include <glm/glm.hpp>
#include "base/tapering.h"
#include "base/column.h"
#include "base/draw_object.h"

class Grain : DrawObject {
public:
    Grain();
    ~Grain() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Tapering tapering_;
    Column column_;
    glm::vec3 ka_ = glm::vec3(0.05f, 0.05f, 0.05f); // �����ⷴ��
    glm::vec3 kd_ = glm::vec3(0.2f, 0.4f, 0.4f);   // ������ (�ӽ���ɫ)
    glm::vec3 ks_ = glm::vec3(0.1f, 0.1f, 0.1f);   // ���淴�� (΢���Ĺ���)
    float ns_ = 10.0f;                             // �߹�ָ�� (��΢�Ĵֲڸ�)
};
#endif // !GRAIN_H