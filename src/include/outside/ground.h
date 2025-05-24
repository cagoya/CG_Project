#ifndef GROUND_H
#define GROUND_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include"base/square.h"
#include "base/draw_object.h"

// µÿ√Ê
class Ground : DrawObject {
public:
    Ground();
    ~Ground() = default;
	void setup();

	void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
	Square square_;
};

#endif // GROUND_H