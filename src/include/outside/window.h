#ifndef WINODW_H
#define WINDOW_H

#include <glm/glm.hpp>
#include "base/draw_object.h"
#include "base/square.h"

// 窗户
class Window:DrawObject {
public:
    Window();
    ~Window() = default;

    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix) const;
private:
    Square square_;
    glm::vec3 ka_ = glm::vec3(0.08f, 0.08f, 0.08f); // 环境光反射 (提供基础亮度)
    glm::vec3 kd_ = glm::vec3(0.85f, 0.85f, 0.85f); // 漫反射 (明亮的白色)
    glm::vec3 ks_ = glm::vec3(0.2f, 0.2f, 0.2f);   // 镜面反射 (轻微的光泽，模拟塑料或漆面)
    float ns_ = 40.0f;                             // 高光指数 (适中，高光不至于太散)
};
#endif