#ifndef FLOOR_H
#define FLOOR_H

#include <glm/glm.hpp>
#include "base/square.h"
#include "base/draw_object.h"

//室内地板
class Floor : DrawObject {
public:
    Floor();
    ~Floor() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix,bool useTexture) const;
private:
    Square square_;
    glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.07f); // 环境光反射 (略带暖色调)
    glm::vec3 kd_ = glm::vec3(0.7f, 0.6f, 0.4f);   // 漫反射 (米黄色)
    glm::vec3 ks_ = glm::vec3(0.5f, 0.5f, 0.5f); // 镜面反射 (中等光泽)
    float ns_ = 90.0f;                             // 高光指数 (适中，模拟木地板或瓷砖)
};
#endif // !FLOOR_H