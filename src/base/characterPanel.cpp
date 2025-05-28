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

        ImGui::Text("Please enter characters:");
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

        ImGui::End();
    }
}
