#include "outside/ground.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include<stb_image.h>

// 一个正方形作为地面，颜色为绿色，之后贴图为草地
static const float groundVertices[] = {
    // 位置                  // 纹理坐标
    -100.0f, 0.01f, -100.0f,  0.0f, 100.0f,
     100.0f, 0.01f, -100.0f, 100.0f, 100.0f,
     100.0f, 0.01f,  100.0f, 100.0f,  0.0f,
    -100.0f, 0.01f,  100.0f,  0.0f,  0.0f
};

static const unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};


Ground::Ground() : DrawObject()
{
    if (textureID_) {
        glDeleteTextures(1, &textureID_);
    }
}

Ground::~Ground() {}

// 绑定 vao_ vbo_ ebo_ 
void Ground::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // 纹理加载
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    // 设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S轴重复
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T轴重复

    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 缩小使用Mipmap线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);             // 放大使用线性过滤

    // 加载图像数据
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // 大多数图像格式的原点在左上角，OpenGL纹理坐标原点在左下角，所以需要翻转
    unsigned char* data = stbi_load("../../media/textures/grass.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Texture format not supported: " << nrChannels << " channels" << std::endl;
            stbi_image_free(data);
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture: grass.jpg" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// 绘制地面
void Ground::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);
    glActiveTexture(GL_TEXTURE0); // 激活纹理单元0
    glBindTexture(GL_TEXTURE_2D, textureID_);
    shader.setInt("texture", 0);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}