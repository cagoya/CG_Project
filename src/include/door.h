#ifndef DOOR_H
#define DOOR_H

#include <glm/glm.hpp>
#include "draw_object.h"

// це
class Door : DrawObject {
public:
    Door();
    ~Door();

    void setup();
    void draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const;
};
#endif // !DOOR_H