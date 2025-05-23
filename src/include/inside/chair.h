#ifndef CHAIR_H
#define CHAIR_H

#include "base\shader.h"
#include "base\draw_object.h"
#include <glm/glm.hpp> 

class Chair:DrawObject {
public:
    Chair();
    ~Chair();

    void setup();
    void draw(Shader shader, const glm::mat4& modelMatrix) const;
};

#endif // CHAIR_H