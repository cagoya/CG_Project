#ifndef MODELTRANSFORMPANEL_H
#define MODELTRANSFORMPANEL_H

#include "IImGuiPanel.h" 
#include <glm/glm.hpp>     
#include <glm/gtc/type_ptr.hpp> 
// 前向声明 ImGuiIO，避免在头文件中包含 "imgui_internal.h" 或过多 ImGui 细节 

class ModelTransformPanel : public IImGuiPanel {
public:
    // 引用操作 main.cpp 里面的变量
    glm::vec3& modelPositionRef;
    float& modelScaleRef;

    ModelTransformPanel(const char* name, glm::vec3& positionRef, float& scaleRef, ImGuiIO& io)
        : modelPositionRef(positionRef), modelScaleRef(scaleRef) {
        PanelName = name; 
        IsVisible = true;   // 默认可见
    }

    void Draw() override; 
};

#endif 