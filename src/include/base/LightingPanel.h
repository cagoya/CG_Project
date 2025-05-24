#ifndef LIGHTINGPANEL_H
#define LIGHTINGPANEL_H

#include "IImGuiPanel.h"
#include "base/light.h"
#include <glm/glm.hpp>     
#include <glm/gtc/type_ptr.hpp> 
// ǰ������ ImGuiIO��������ͷ�ļ��а��� "imgui_internal.h" ����� ImGui ϸ�� 

class LightingPanel : public IImGuiPanel {
public:
    // ���ò��� main.cpp ����ı���
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