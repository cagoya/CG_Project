//����ʵ�� ����������Ӹ����߼�
#include "base/LightingPanel.h"

void LightingPanel::Draw() {
    if (!IsVisible) {
        return;
    }
    if (ImGui::Begin(PanelName, &IsVisible)) { // ��ʼһ�� ImGui ����,����ͨ�����ڵĹرհ�ť���ı� IsVisible ��ֵ
        ImGui::Text("Render Mode");
        ImGui::Separator();

        ImGui::Text("ambient light");
        ImGui::Separator();
        ImGui::SliderFloat("intensity##1", &ambientLightRef.intensity, 0.0f, 5.0f);
        ImGui::ColorEdit3("color##1", (float*)&ambientLightRef.color);
        ImGui::NewLine();

        ImGui::Text("directional light");
        ImGui::Separator();
        ImGui::SliderFloat("intensity##2", &directionalLightRef.intensity, 0.0f, 5.0f);
        ImGui::ColorEdit3("color##2", (float*)&directionalLightRef.color);
		ImGui::SliderFloat3("direction##2", (float*)&directionalLightRef.direction, -1.0f, 1.0f);
        ImGui::NewLine();

        ImGui::Text("spot light");
        ImGui::Separator();
        ImGui::SliderFloat("intensity##3", &spotLightRef.intensity, 0.0f, 5.0f);
        ImGui::ColorEdit3("color##3", (float*)&spotLightRef.color);
        ImGui::SliderFloat(
            "angle##3", (float*)&spotLightRef.angle, 0.0f, glm::radians(180.0f), "%f rad");
        ImGui::NewLine();
    }
    ImGui::End();
}