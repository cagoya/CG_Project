#ifndef WINODW_H
#define WINDOW_H

#include <glm/glm.hpp>
#include "base/draw_object.h"
#include "base/square.h"

// ����
class Window:DrawObject {
public:
    Window();
    ~Window() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
    glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.08f); // �����ⷴ�� (�ṩ��������)
    glm::vec3 kd_ = glm::vec3(0.85f, 0.85f, 0.85f); // ������ (�����İ�ɫ)
    glm::vec3 ks_ = glm::vec3(0.2f, 0.2f, 0.2f);   // ���淴�� (��΢�Ĺ���ģ�����ϻ�����)
    float ns_ = 40.0f;                             // �߹�ָ�� (���У��߹ⲻ����̫ɢ)
};
#endif