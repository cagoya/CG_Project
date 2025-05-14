/* 所有面板通用接口 但是包含面板名字和是否可见 所以不算很严谨 可以再改动！
* 析构不说了 draw方法是要每种面板实现一遍的！
*/
#ifndef IIMGUIPANEL_H
#define IIMGUIPANEL_H

#include "imgui.h"

class IImGuiPanel {
public:
    virtual ~IImGuiPanel() = default; // OOP这下用到了

    // 纯虚函数 每种面板都要实现Draw -多态
    virtual void Draw() = 0;

    bool IsVisible = true;        // 控制面板是否可见
    const char* PanelName = "Panel"; // 默认名称
};

#endif 