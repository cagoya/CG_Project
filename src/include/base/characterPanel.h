#ifndef CHARACTERPANEL_H
#define CHARACTERPANEL_H
#include "IImGuiPanel.h"
#include <string>

class CharacterPanel : public IImGuiPanel {
  public:
    std::string& characterRef;
    CharacterPanel(const char* name, ImGuiIO& io, std::string& characterRef):characterRef(characterRef) {
        PanelName = name;
        IsVisible = true;
    }

    void Draw() override;
};
#endif //CHARACTERPANEL_H
