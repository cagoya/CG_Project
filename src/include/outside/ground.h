#ifndef GROUND_H
#define GROUND_H

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "base/draw_object.h"

// µÿ√Ê
class Ground : DrawObject {
public:
    Ground();
    ~Ground();
	void setup();

	void draw(Shader shader, const glm::mat4& modelMatrix) const;
private:
	GLuint textureID_ = 0;
};

#endif // GROUND_H