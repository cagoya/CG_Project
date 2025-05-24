#ifndef FLOOR_H
#define FLOOR_H

#include <glm/glm.hpp>
#include "base/square.h"
#include "base/draw_object.h"

//ÊÒÄÚµØ°å
class Floor : DrawObject {
public:
    Floor();
    ~Floor() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
};
#endif // !FLOOR_H