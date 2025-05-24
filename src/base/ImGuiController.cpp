#include "base/ImGuiController.h"
#include <iostream>

ImGuiController::ImGuiController() {}

ImGuiController::~ImGuiController() {
    if (initialized) { // 确保只在初始化后才执行 Shutdown
         Shutdown();
    }
}

void ImGuiController::Init(GLFWwindow* window, const char* glsl_version) {
    if (initialized) return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        std::cerr << "Failed to initialize ImGui GLFW backend!" << std::endl;
        return;
    }
    if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
        std::cerr << "Failed to initialize ImGui OpenGL3 backend!" << std::endl;
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        return;
    }
    initialized = true;
    std::cout << "ImGui initialized successfully by ImGuiController." << std::endl;
}

void ImGuiController::NewFrame() {
    if (!initialized) return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiController::DrawUI() {
    if (!initialized) return;
    for (const auto& panel : panels) {
        if (panel) {
            panel->Draw(); // 调用每个已注册面板的 Draw 方法
        }
    }
}

void ImGuiController::Render() {
    if (!initialized) return;
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiController::Shutdown() {
    if (!initialized) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    initialized = false;
    std::cout << "ImGui shutdown by ImGuiController." << std::endl;
}

void ImGuiController::AddPanel(const std::shared_ptr<IImGuiPanel>& panel) {
    panels.push_back(panel);
}