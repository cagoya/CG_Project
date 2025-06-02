
#include "base/characterPanel.h"
#include <string>

void CharacterPanel::Draw() {
    if (!IsVisible) {
        return;
    }
    if (ImGui::Begin(PanelName, &IsVisible)) {

        static char inputBuffer[256] = ""; // 假设最大输入长度为255个字符 + null terminator
        // 如果 characterRef 已经有内容，将其复制到缓冲区
        if (!characterRef.empty()) {
            strncpy(inputBuffer, characterRef.c_str(), sizeof(inputBuffer) - 1);
            inputBuffer[sizeof(inputBuffer) - 1] = '\0'; // 确保字符串以 null 结尾
        }

        ImGui::Text("please input character");
        if (ImGui::InputText("##CharacterInput", inputBuffer, sizeof(inputBuffer))) {
            // 当输入内容改变时，更新 characterRef
            characterRef = inputBuffer;
        }

        // 你可以在这里添加其他 UI 元素
        ImGui::Text("%s", characterRef.c_str());

        if (ImGui::Button("clear")) {
            characterRef.clear();
            inputBuffer[0] = '\0'; // 同时清除缓冲区
        }

        // 下拉列表选择字体
        const char* items[] = { "宋体", "仿宋", "黑体", "楷体", "隶书" };
        const char* previewValue = items[font_choiceRef]; // 在下拉列表关闭时显示的值

        ImGui::Text("请选择一种字体：");
        if (ImGui::BeginCombo("##MyCombo", previewValue)) { // "##MyCombo" 是唯一的ID，previewValue 是默认显示的值
            for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
                const bool isSelected = (font_choiceRef == i); // 判断当前项是否被选中
                if (ImGui::Selectable(items[i], isSelected)) { // ImGui::Selectable 返回 true 如果被点击
                    font_choiceRef = i; // 更新选中项的索引
                }
                // 如果当前项被选中，自动滚动到可见区域
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Text("目前的字体是： %s", items[font_choiceRef]);

        // 选择字体大小
        ImGui::SliderFloat("调整字体大小", &(font_sizeRef), 10.0f, 100.0f, "当前值: %.1f");

        // 调整字体颜色
        ImGui::Text("调整字体颜色:");
        ImGui::SliderInt("R", &R_Ref, 0, 255, "当前值: %d");
        ImGui::SliderInt("G", &G_Ref, 0, 255, "当前值: %d");
        ImGui::SliderInt("B", &B_Ref, 0, 255, "当前值: %d");

        ImGui::End();
    }
}
