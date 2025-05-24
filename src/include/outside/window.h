#ifndef WINODW_H
#define WINDOW_H

#include <glm/glm.hpp>
#include "base/draw_object.h"
#include "base/square.h"

// ´°»§
class Window:DrawObject {
public:
    Window();
    ~Window() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
};
#endif