#ifndef GABLE_ROOF_H
#define GABLE_ROOF_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "base/draw_object.h"

// 一个简单的双坡屋顶
class GableRoof : DrawObject{
public:
    GableRoof();
    ~GableRoof();

    void setup();

    void draw(Shader shader, const glm::mat4& modelMatrix) const;
private:
    GLuint textureID_ = 0;
};

#endif // GABLE_ROOF_H