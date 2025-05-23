#ifndef DOOR_H
#define DOOR_H

#include <glm/glm.hpp>
#include "base/draw_object.h"

// це
class Door : DrawObject {
public:
    Door();
    ~Door();

    void setup();
    void draw(Shader shader, const glm::mat4& modelMatrix) const;
private:
    GLuint textureID_;
};
#endif // !DOOR_H