#ifndef GABLE_ROOF_H
#define GABLE_ROOF_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "base/square.h"
#include "base/draw_object.h"

// 一个简单的双坡屋顶
class GableRoof : DrawObject{
public:
    GableRoof();
    ~GableRoof() = default;

    void setup();

    void draw(Shader shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
    glm::vec3 ka_ = glm::vec3(0.2f, 0.02f, 0.02f);
    glm::vec3 kd_ = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 ks_ = glm::vec3(0.5f, 0.5f, 0.5f);
    float ns_ = 32.0f;
};

#endif // GABLE_ROOF_H