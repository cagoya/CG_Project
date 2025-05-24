/* ImGuiController ����ImGui����������
Init ���÷�� ��ʼ��ImGui���˰�
NewFrame():ÿһ֡�����²� NewFrame
Render() ��������UI
Shutdown() �ر�ImGui �ͷ���Դ
���ܴ�������Ŀؼ� ���ǹ���һ��panel�����б� add���Լ���� drawUI����ͳһ����
 */
#ifndef IMGUICONTROLLER_H
#define IMGUICONTROLLER_H

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "IImGuiPanel.h" // �����ӿ�

class ImGuiController {
private:
    std::vector<std::shared_ptr<IImGuiPanel>> panels; // �洢���� UI �����б�
    bool initialized = false;

public:
    ImGuiController();
    ~ImGuiController();

    void Init(GLFWwindow* window, const char* glsl_version = "#version 330 core");
    void NewFrame();
    void DrawUI(); // ��������ע������ Draw() ����
    void Render();
    void Shutdown();

    // �����嵽������
    void AddPanel(const std::shared_ptr<IImGuiPanel>& panel);

    // ��ȡ ImGuiIO �����ã��������������ط�ʹ��
    ImGuiIO& GetIO() { return ImGui::GetIO(); }
};

#endif 