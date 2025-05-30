#include "base/ModelTransformPanel.h"

void ModelTransformPanel::Draw() {
    if (!IsVisible) { 
        return;
    }
    if (ImGui::Begin(PanelName, &IsVisible)) { // ��ʼһ�� ImGui ����,����ͨ�����ڵĹرհ�ť���ı� IsVisible ��ֵ
        ImGui::Text("Model transform control:");
        ImGui::SliderFloat("Scale", &modelScaleRef, 0.001f, 1.0f);
        ImGui::InputFloat3("Position", glm::value_ptr(modelPositionRef));
        ImGui::Spacing();
    }
    ImGui::End();
}