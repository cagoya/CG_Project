#ifndef GROUND_H
#define GROUND_H

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "draw_object.h"

// µÿ√Ê
class Ground : DrawObject {
public:
    Ground();
    ~Ground();
	void setup();

	void draw(Shader shader, const glm::mat4& modelMatrix) const;
};

#endif // GROUND_H