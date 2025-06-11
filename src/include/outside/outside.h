// 室内物品的绘制
#include <string>

#include "outside/wall.h"
#include "outside/ground.h"
#include "outside/roof.h"
#include "outside/door.h"
#include "outside/window.h"
#include "outside/fence.h"
#include "outside/path.h"
#include "outside/stone.h"
#include "outside/tree.h"

class Outside {
public:
    Outside() = default;
    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix, bool useTexture);
private:
    Wall wall_;
    Ground ground_;
    Path path_;
    Fence fence_;
    Stone stone_;
    Tree tree_;
    Roof roof_;
    Door door_;
    Window window_;
};