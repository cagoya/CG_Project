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
};

#endif // GABLE_ROOF_H