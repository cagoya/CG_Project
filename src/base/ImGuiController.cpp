#include "base/ImGuiController.h"
#include <iostream>

ImGuiController::ImGuiController() {}

ImGuiController::~ImGuiController() {
    if (initialized) {
         Shutdown();
    }
}

void ImGuiController::Init(GLFWwindow* window, const char* glsl_version) {
    if (initialized) return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // 设置字体配置，用于合并中文字符
    ImFontConfig font_config;

    static const ImWchar chinese_ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuation, Hiragana, Katakana
        0x31F0, 0x31FF, // Kanbun
        0x4e00, 0x9FFF, // CJK Unified Ideographs
        0xff00, 0xffef, // Halfwidth and Fullwidth Forms
        0,
    };

    // 使用自定义的字符范围
    ImFont* font = io.Fonts->AddFontFromFileTTF("../../media/font/simfang.ttf", 18.0f, &font_config, chinese_ranges);

    if (font) {
        io.FontDefault = font; // 设置为默认字体
    } else {
        // 处理字体加载失败的情况，例如打印错误信息
        printf("Error: Failed to load Chinese font!\n");
    }

    // 构建字体图集，这在应用程序启动后只需要调用一次
    io.Fonts->Build();
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
            panel->Draw(); // ����ÿ����ע������ Draw ����
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