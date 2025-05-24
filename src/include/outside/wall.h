#ifndef WALL_H
#define WALL_H


#include <glm/glm.hpp>

#include "base/draw_object.h"
#include "base/square.h"

class Wall: DrawObject{
public:
    Wall();
    ~Wall() = default;

    void setup();

    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
    glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.08f); // �����ⷴ�� (�Դ�����)
    glm::vec3 kd_ = glm::vec3(0.5f, 0.5f, 0.5f);   // ������ (�л�ɫ)
    glm::vec3 ks_ = glm::vec3(0.02f, 0.02f, 0.02f); // ���淴�� (�ǳ�΢��������û�й���)
    float ns_ = 5.0f;                             // �߹�ָ�� (�ǳ���ɢ��ģ��ֲ�ǽ��)
};

#endif