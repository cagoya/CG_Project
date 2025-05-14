/* ImGuiController 管理ImGui的整个周期
Init 设置风格 初始化ImGui与后端绑定
NewFrame():每一帧调用下层 NewFrame
Render() 绘制总体UI
Shutdown() 关闭ImGui 释放资源
不管创建具体的控件 就是管理一个panel面板的列表 add可以加面板 drawUI可以统一绘制
 */
#ifndef IMGUICONTROLLER_H
#define IMGUICONTROLLER_H

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "IImGuiPanel.h" // 含面板接口

class ImGuiController {
private:
    std::vector<std::shared_ptr<IImGuiPanel>> panels; // 存储所有 UI 面板的列表
    bool initialized = false;

public:
    ImGuiController();
    ~ImGuiController();

    void Init(GLFWwindow* window, const char* glsl_version = "#version 330 core");
    void NewFrame();
    void DrawUI(); // 调用所有注册面板的 Draw() 方法
    void Render();
    void Shutdown();

    // 添加面板到控制器
    void AddPanel(const std::shared_ptr<IImGuiPanel>& panel);

    // 获取 ImGuiIO 的引用，方便面板或其他地方使用
    ImGuiIO& GetIO() { return ImGui::GetIO(); }
};

#endif 