#ifndef CUBE_H
#define CUBE_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "draw_object.h"

// ǽ
class Wall: DrawObject{
public:
    Wall();
    ~Wall();

    void setup();

    void draw(Shader shader, const glm::mat4& modelMatrix) const;
};

#endif // CUBE_H