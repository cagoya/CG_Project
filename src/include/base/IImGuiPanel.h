#ifndef IIMGUIPANEL_H
#define IIMGUIPANEL_H

#include "imgui.h"

class IImGuiPanel {
public:
    virtual ~IImGuiPanel() = default;

    virtual void Draw() = 0;
    bool IsVisible = true;
    const char* PanelName = "Panel";
};

#endif 