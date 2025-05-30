#ifndef LIGHTINGPANEL_H
#define LIGHTINGPANEL_H

#include "IImGuiPanel.h"
#include "base/light.h"
#include <glm/glm.hpp>     
#include <glm/gtc/type_ptr.hpp> 
// 前向声明 ImGuiIO，避免在头文件中包含 "imgui_internal.h" 或过多 ImGui 细节 

class LightingPanel : public IImGuiPanel {
public:
    // 引用操作 main.cpp 里面的变量
	AmbientLight& ambientLightRef;
    SpotLight& spotLightRef;
    DirectionalLight& directionalLightRef;
    LightingPanel(const char* name,ImGuiIO& io,SpotLight& spotlightref, AmbientLight& ambientlightref, DirectionalLight
        & directionalightref,Material& materialref) 
        : ambientLightRef(ambientlightref),spotLightRef(spotlightref),directionalLightRef(directionalightref)
    {
        PanelName = name;
        IsVisible = true;
    }

    void Draw() override;
};

#endif 