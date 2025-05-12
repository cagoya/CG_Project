#ifndef GABLE_ROOF_H
#define GABLE_ROOF_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "draw_object.h"

// 一个简单的双坡屋顶
class GableRoof : DrawObject{
public:
    GableRoof();
    ~GableRoof();

    void setup();

    void draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const;
};

#endif // GABLE_ROOF_H