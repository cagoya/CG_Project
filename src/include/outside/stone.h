#ifndef STONE_H
#define STONE_H


#include <glm/glm.hpp>

#include "base/draw_object.h"
#include "base/column.h"
#include "base/sphere.h"

class Stone : DrawObject {
public:
    Stone();
    ~Stone() = default;

    void setup();

    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Column column_;
    Sphere sphere_;
    glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.08f); // 环境光反射 (略带亮度)
    glm::vec3 kd_ = glm::vec3(0.5f, 0.5f, 0.5f);   // 漫反射 (中灰色)
    glm::vec3 ks_ = glm::vec3(0.02f, 0.02f, 0.02f); // 镜面反射 (非常微弱，几乎没有光泽)
    float ns_ = 5.0f;                             // 高光指数 (非常分散，模拟粗糙墙面)
};

#endif