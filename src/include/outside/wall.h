#ifndef WALL_H
#define WALL_H


#include <glm/glm.hpp>

#include "base/draw_object.h"
#include "base/square.h"

class Wall: DrawObject{
public:
    Wall();
    ~Wall() = default;

    void setup();

    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
};

#endif