#ifndef CHARACTERPANEL_H
#define CHARACTERPANEL_H
#include "IImGuiPanel.h"
#include <string>
#include <vector>

class CharacterPanel : public IImGuiPanel {
  public:
    std::string& characterRef;
    int& font_choiceRef;
    float& font_sizeRef;
    int& R_Ref;
    int& G_Ref;
    int& B_Ref;
    CharacterPanel(const char* name, ImGuiIO& io, std::string& characterRef, int& font_choice, float& font_size, int& R, int& G, int& B):
    characterRef(characterRef), font_choiceRef(font_choice), font_sizeRef(font_size),R_Ref(R), G_Ref(G), B_Ref(B)  {
        PanelName = name;
        IsVisible = true;
    }

    void Draw() override;
};
#endif //CHARACTERPANEL_H
