// 室内物品的绘制
#include <string>

#include "inside/floor.h"
#include "inside/table.h"
#include "inside/chair.h"
#include "inside/clock.h"
#include "inside/calligraphy.h"

class Inside {
public:
	Inside() = default;
    void setup();
    void draw(Shader& shader, const glm::mat4& modelMatrix, std::string& sentence, std::string font, float font_size, int R, int G, int B, bool useTexture);
    void drawShadow(Shader& shader, const glm::mat4& modelMatrix, std::string& sentence, std::string font, float font_size, int R, int G, int B, bool useTexture);
private:
    Floor floor_;
    Table table_;
    Chair chair_;
    Clock clock_;
    Calligraphy calligraphy_;
	
};