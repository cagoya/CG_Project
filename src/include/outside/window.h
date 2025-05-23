#ifndef WINODW_H
#define WINDOW_H

#include <glm/glm.hpp>
#include "base/draw_object.h"

// ´°»§
class Window:DrawObject {
public:
    Window();
    ~Window();

    void setup();
    void draw(Shader shader, const glm::mat4& modelMatrix) const;
private:
    GLuint textureID_ = 0;
};
#endif