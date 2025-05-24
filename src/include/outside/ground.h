#ifndef GROUND_H
#define GROUND_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include"base/square.h"
#include "base/draw_object.h"

// ����
class Ground : DrawObject {
public:
    Ground();
    ~Ground() = default;
	void setup();

	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
    glm::vec3 ka_ = glm::vec3(0.1f, 0.2f, 0.05f);
    glm::vec3 kd_ = glm::vec3(0.2f, 0.6f, 0.1f);
    glm::vec3 ks_ = glm::vec3(0.05f, 0.05f, 0.05f); // ���Գ��Ը��ͻ�0
    float ns_ = 5.0f;
};

#endif // GROUND_H