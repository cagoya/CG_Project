#ifndef FLOOR_H
#define FLOOR_H

#include <glm/glm.hpp>
#include "draw_object.h"

//ÊÒÄÚµØ°å
class Floor : DrawObject {
public:
    Floor();
    ~Floor();

    void setup();
    void draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const;
};
#endif // !FLOOR_H