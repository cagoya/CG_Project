#ifndef WINODW_H
#define WINDOW_H

#include <glm/glm.hpp>
#include "draw_object.h"

// ´°»§
class Window:DrawObject {
public:
    Window();
    ~Window();

    void setup();
    void draw(unsigned int shaderProgramID, const glm::mat4& modelMatrix) const;
};
#endif