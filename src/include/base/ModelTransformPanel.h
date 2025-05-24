#ifndef MODELTRANSFORMPANEL_H
#define MODELTRANSFORMPANEL_H

#include "IImGuiPanel.h" 
#include <glm/glm.hpp>     
#include <glm/gtc/type_ptr.hpp> 
// ǰ������ ImGuiIO��������ͷ�ļ��а��� "imgui_internal.h" ����� ImGui ϸ�� 

class ModelTransformPanel : public IImGuiPanel {
public:
    // ���ò��� main.cpp ����ı���
    glm::vec3& modelPositionRef;
    float& modelScaleRef;

    ModelTransformPanel(const char* name, glm::vec3& positionRef, float& scaleRef, ImGuiIO& io)
        : modelPositionRef(positionRef), modelScaleRef(scaleRef) {
        PanelName = name; 
        IsVisible = true;   // Ĭ�Ͽɼ�
    }

    void Draw() override; 
};

#endif 