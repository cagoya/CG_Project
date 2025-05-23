#include "outside/Window.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "stb_image.h"

// �������㣺�����Ҳ�ǽ�棬����
static const float windowVertices[] = {
    // λ��              // ��������
    0.501f, 0.175f, -0.15f,   0.0f, 0.0f, // ����
    0.501f, 0.175f,  0.15f,   1.0f, 0.0f, // ����
    0.501f, 0.325f,  0.15f,   1.0f, 1.0f, // ����
    0.501f, 0.325f, -0.15f,   0.0f, 1.0f  // ����
};

static const unsigned int windowIndices[] = {
    0, 1, 2,
    0, 2, 3
};

Window::Window() : DrawObject() {}

Window::~Window()
{
    if (textureID_ != 0) {
        glDeleteTextures(1, &textureID_);
    }
}

void Window::setup() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowIndices), windowIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // �������
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    // ���������Ʋ���
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S���ظ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T���ظ�

    // ����������˲���
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // ��Сʹ��Mipmap���Թ���
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);             // �Ŵ�ʹ�����Թ���

    // ����ͼ������
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // �����ͼ���ʽ��ԭ�������Ͻǣ�OpenGL��������ԭ�������½ǣ�������Ҫ��ת
    unsigned char* data = stbi_load("../../media/textures/window.png", &width, &height, &nrChannels, 0);
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
        std::cerr << "Failed to load texture: window.png" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Window::draw(Shader shader, const glm::mat4& modelMatrix) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.setMat4("model", modelMatrix);
    shader.setInt("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
