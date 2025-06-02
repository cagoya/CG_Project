#include "inside/calligraphy.h"
#include<iostream>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stb_image_write.h"
#include "utf8.h"

Calligraphy::Calligraphy()
{
    square_.getVertices() = {
        // 位置 (Position)             // 法向量 (Normal)     // 颜色 (Color)         // 纹理坐标 (Texture)
        // 左下 (Bottom-left)
        -0.25f, 0.5f, -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // 右下 (Bottom-right)
        0.25f, 0.5f, -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // 右上 (Top-right)
        0.25f, 0.7f,  -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // 左上 (Top-left)
        -0.25f, 0.7f,  -0.98f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 1.0f
    };

    square_.getIndices() = {
        0, 1, 2,
        0, 2, 3
    };

    square_.getTexturePath() = "../../media/textures/paper.jpg";

    char_square_.getVertices() = {
        // 位置 (Position)             // 法向量 (Normal)     // 颜色 (Color)         // 纹理坐标 (Texture)
        // 左下 (Bottom-left)
        -0.25f, 0.5f, -0.97f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        // 右下 (Bottom-right)
        0.25f, 0.5f, -0.97f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        // 右上 (Top-right)
        0.25f, 0.7f,  -0.97f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // 左上 (Top-left)
        -0.25f, 0.7f,  -0.97f,           0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f,      0.0f, 1.0f
    };

    char_square_.getIndices() = {
        0, 1, 2,
        0, 2, 3
    };
    char_square_.getTexturePath() = "../../media/textures/chinese_sentence_cpp.png";
}

void Calligraphy::setup() {
    square_.setup();
    char_square_.setup();
}

void Calligraphy::draw(Shader& shader, const glm::mat4& modelMatrix) const {
    shader.setVec3("material.ka", ka_);
    shader.setVec3("material.kd", kd_);
    shader.setVec3("material.ks", ks_);
    shader.setFloat("material.ns", ns_);
    square_.draw(shader, modelMatrix);
    // 因为要允许修改文字，所以必须到绘制时再生成并绑定纹理
    char_square_.draw(shader,modelMatrix);
}

void Calligraphy::generateTexture(std::string sentence, std::string font_string, float font_size, int R, int G, int B)
{
    // 1. 加载字体文件，可以使用系统自带的字体，这里只写了windows
    const std::string font_path = "../../media/font/" + font_string;
    FILE* fontFile = fopen(font_path.c_str(), "rb");
    if (!fontFile) {
        std::cerr << "无法打开字体文件: " << font_path << std::endl;
        return;
    }

    fseek(fontFile, 0, SEEK_END);
    long size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);

    std::vector<unsigned char> fontBuffer(size);
    fread(fontBuffer.data(), 1, size, fontFile);
    fclose(fontFile);

    // 2. 初始化字体
    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, fontBuffer.data(), stbtt_GetFontOffsetForIndex(fontBuffer.data(), 0))) {
        std::cerr << "初始化字体失败" << std::endl;
        return;
    }

    // 3. 设置要渲染的字符串和字体大小
    const std::string text_to_render = sentence;
    float scale = stbtt_ScaleForPixelHeight(&font, font_size);

    // 获取字体全局度量信息 (用于计算基线)
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
    ascent = static_cast<int>(roundf(ascent * scale));
    descent = static_cast<int>(roundf(descent * scale));

    // 预计算整个字符串的宽度和高度
    int current_x_advance = 0;
    int max_height = ascent - descent; // 文本的实际高度

    // 使用 utf8::iterator 遍历字符串并计算总宽度
    utf8::iterator<std::string::const_iterator> it(text_to_render.begin(), text_to_render.begin(), text_to_render.end());
    utf8::iterator<std::string::const_iterator> end_it(text_to_render.end(), text_to_render.begin(), text_to_render.end());

    while (it != end_it) {
        unsigned int codepoint = *it; // 获取当前字符的 Unicode 码点

        int glyph_index = stbtt_FindGlyphIndex(&font, codepoint);
        if (glyph_index != 0) {
            int advanceWidth, leftSideBearing;
            stbtt_GetGlyphHMetrics(&font, glyph_index, &advanceWidth, &leftSideBearing);
            current_x_advance += static_cast<int>(roundf(advanceWidth * scale));
        }
        else {
            // 如果字体不包含该字符，尝试使用一个默认字形的宽度（例如，空格）
            // 避免字符重叠，并打印警告
            std::cerr << "警告: 字体不包含字符 U+" << std::hex << std::uppercase << codepoint << std::endl;
            int advanceWidth, leftSideBearing;
            stbtt_GetGlyphHMetrics(&font, stbtt_FindGlyphIndex(&font, ' '), &advanceWidth, &leftSideBearing); // 使用空格的宽度
            current_x_advance += static_cast<int>(roundf(advanceWidth * scale));
        }
        ++it; // 移动到下一个 UTF-8 字符
    }
    int total_text_width = current_x_advance;

    int padding = 10; // 四周留白

    // 最终位图的尺寸
    int bitmap_width = total_text_width + 2 * padding;
    int bitmap_height = max_height + 2 * padding;

    if (bitmap_width <= 0 || bitmap_height <= 0) {
        std::cerr << "计算的位图尺寸无效 (width: " << bitmap_width << ", height: " << bitmap_height << ")。请检查字体文件和文本内容。" << std::endl;
        return ;
    }

    // 创建RGBA格式的位图
    // 使用 std::vector<unsigned char> 来管理内存，更安全
    std::vector<unsigned char> rgba_bitmap(bitmap_width * bitmap_height * 4, 0); // 初始化为0 (透明黑色)

    // 4. 渲染字符串
    int current_draw_x = padding; // 从留白后开始绘制
    int baseline_y = padding + ascent; // 基线位置

    // 再次使用 utf8::iterator 遍历字符串进行实际渲染
    it = utf8::iterator<std::string::const_iterator>(text_to_render.begin(), text_to_render.begin(), text_to_render.end());
    while (it != end_it) {
        unsigned int codepoint = *it; // 获取当前字符的 Unicode 码点

        int glyph_index = stbtt_FindGlyphIndex(&font, codepoint);
        if (glyph_index == 0) {
            // 已经打印过警告，这里只需处理前进宽度
            int advanceWidth, leftSideBearing;
            stbtt_GetGlyphHMetrics(&font, stbtt_FindGlyphIndex(&font, ' '), &advanceWidth, &leftSideBearing);
            current_draw_x += static_cast<int>(roundf(advanceWidth * scale));
            ++it;
            continue;
        }

        int x0, y0, x1, y1;
        stbtt_GetGlyphBitmapBox(&font, glyph_index, scale, scale, &x0, &y0, &x1, &y1);

        int char_width = x1 - x0;
        int char_height = y1 - y0;

        // 为当前字符创建单色位图
        std::vector<unsigned char> mono_bitmap(char_width * char_height);
        stbtt_MakeGlyphBitmap(&font, mono_bitmap.data(), char_width, char_height, char_width, scale, scale, glyph_index);

        // 将单色位图转换为RGBA位图并合并
        for (int y = 0; y < char_height; ++y) {
            for (int x = 0; x < char_width; ++x) {
                int mono_idx = y * char_width + x;
                // 计算在最终大位图上的像素位置
                int draw_y = baseline_y + y0 + y; // y0 是相对基线的偏移
                int draw_x = current_draw_x + x0 + x; // x0 是相对当前绘制位置的偏移

                // 确保在位图范围内
                if (draw_x >= 0 && draw_x < bitmap_width &&
                    draw_y >= 0 && draw_y < bitmap_height) {
                    int rgba_idx = (draw_y * bitmap_width + draw_x) * 4;

                    // 设置颜色(这里用黑色文字)
                    rgba_bitmap[rgba_idx + 0] = R;    // R
                    rgba_bitmap[rgba_idx + 1] = G;    // G
                    rgba_bitmap[rgba_idx + 2] = B;    // B
                    // 使用字形位图值作为 Alpha (透明度) 通道
                    rgba_bitmap[rgba_idx + 3] = mono_bitmap[mono_idx];
                }
            }
        }

        // 获取当前字符的水平前进宽度并累加
        int advanceWidth, leftSideBearing;
        stbtt_GetGlyphHMetrics(&font, glyph_index, &advanceWidth, &leftSideBearing);
        current_draw_x += static_cast<int>(roundf(advanceWidth * scale)); // 移动到下一个字符的起始位置

        ++it; // 移动到下一个 UTF-8 字符
    }

    // 5. 保存为PNG文件
    const std::string output_file = "../../media/textures/chinese_sentence_cpp.png";
    if (!stbi_write_png(output_file.c_str(), bitmap_width, bitmap_height, 4, rgba_bitmap.data(), bitmap_width * 4)) {
        std::cerr << "保存图片失败" << std::endl;
    }
    else {
        //std::cout << "成功保存图片到: " << output_file << std::endl;
    }
}