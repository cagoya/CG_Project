#ifndef DOOR_H
#define DOOR_H

#include <glm/glm.hpp>
#include "base/square.h"
#include "base/draw_object.h"

class Door : DrawObject {
public:
    Door();
    ~Door() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
};
#endif // !DOOR_H