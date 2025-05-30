#ifndef GROUND_H
#define GROUND_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include"base/square.h"
#include "base/draw_object.h"

// 地面
class Ground : DrawObject {
public:
    Ground();
    ~Ground() = default;
	void setup();

	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
    glm::vec3 ka_ = glm::vec3(0.1f, 0.2f, 0.05f);
    glm::vec3 kd_ = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 ks_ = glm::vec3(0.05f, 0.05f, 0.05f); // 可以尝试更低或0
    float ns_ = 5.0f;
};

#endif // GROUND_H