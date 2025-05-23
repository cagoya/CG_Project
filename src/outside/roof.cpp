#include "outside/roof.h"
#include <iostream>
#include <vector>
#include<iostream>
#include<stb_image.h>
#include <glm/gtc/type_ptr.hpp>

GableRoof::GableRoof() : DrawObject() {}

GableRoof::~GableRoof()
{
    if (textureID_ != 0) {
        glDeleteTextures(1, &textureID_);
    }
}

void GableRoof::setup() {
    std::vector<float> vertices = {
        // ��б�����
        -0.6f, 0.475f,  0.6f,  0.0f, 0.0f, // 0: ���ǰ
        -0.6f, 0.475f, -0.6f,  1.0f, 0.0f, // 1: ��׺�
         0.0f,  0.625f, -0.6f,  1.0f, 1.0f, // 2: �ݼ���
         0.0f,  0.625f,  0.6f,  0.0f, 1.0f, // 3: �ݼ�ǰ

         // ��б�����
          0.6f, 0.475f,  0.6f,  0.0f, 0.0f, // 4: �ҵ�ǰ
          0.6f, 0.475f, -0.6f,  1.0f, 0.0f, // 5: �ҵ׺�
          0.0f,  0.625f, -0.6f,  1.0f, 1.0f, // 6: �ݼ��� (ͬ 2)
          0.0f,  0.625f,  0.6f,  0.0f, 1.0f  // 7: �ݼ�ǰ (ͬ 3)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);


    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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
    unsigned char* data = stbi_load("../../media/textures/roof.jpg", &width, &height, &nrChannels, 0);
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
        std::cerr << "Failed to load texture: roof.jpg" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GableRoof::draw(Shader shader, const glm::mat4& modelMatrix) const {
    shader.setMat4("model", modelMatrix);
    glActiveTexture(GL_TEXTURE0); // ��������Ԫ0
    glBindTexture(GL_TEXTURE_2D, textureID_);
    shader.setInt("texture", 0);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}