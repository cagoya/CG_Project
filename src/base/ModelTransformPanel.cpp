//具体实现 可以自行添加更多逻辑
#include "ModelTransformPanel.h"

void ModelTransformPanel::Draw() {
    if (!IsVisible) { 
        return;
    }
    if (ImGui::Begin(PanelName, &IsVisible)) { // 开始一个 ImGui 窗口,允许通过窗口的关闭按钮来改变 IsVisible 的值
        ImGui::Text("Model transform control:");
        ImGui::SliderFloat("Scale", &modelScaleRef, 0.001f, 1.0f);
        ImGui::InputFloat3("Position", glm::value_ptr(modelPositionRef));
        ImGui::Spacing();
    }
    ImGui::End();
}