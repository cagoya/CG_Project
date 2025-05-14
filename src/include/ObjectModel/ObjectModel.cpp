#define TINYOBJLOADER_IMPLEMENTATION // ��һ�� .cpp �ļ��ж��壬������ ObjectModel.cpp
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION // ��һ�� .cpp �ļ��ж���
#include "stb_image.h"

#include "ObjectModel.h"
#include <iostream>
#include <map>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

ObjectModel::ObjectModel() : VAO(0), VBO(0), EBO(0) {}

ObjectModel::~ObjectModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    for (GLuint textureID : textureIDs) {
        glDeleteTextures(1, &textureID);
    }
}

bool ObjectModel::load(const std::string& path, const std::string& mtlBasePath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), mtlBasePath.c_str(), true)) {
        std::cerr << "Failed to load/parse .obj file: " << path << std::endl;
        if (!warn.empty()) std::cerr << "WARNING: " << warn << std::endl;
        if (!err.empty()) std::cerr << "ERROR: " << err << std::endl;
        return false;
    }

    if (!warn.empty()) {
        std::cout << "OBJ Loader WARNING: " << warn << std::endl;
    }

    // ��վ�����
    vertices.clear();
    indices.clear();
    for (GLuint textureID : textureIDs) {
        glDeleteTextures(1, &textureID);
    }
    textureIDs.clear();

    std::map<std::tuple<int, int, int>, unsigned int> uniqueVertices{};

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f]; // ͨ����3����Ϊ�������������ǻ�

            // TODO: Ŀǰ������װ汾����һ�� OBJ ֻ��һ������/����
            // �����Ҫ֧��ÿ�����в�ͬ���ʣ�����Ҫ�����ӵ��߼������鶥��/��������
            // ��������ֻ���Լ��ص�һ�����ʵ���������������еĻ���
            if (!materials.empty() && textureIDs.empty()) { // ֻ����һ������ʾ��
                if (!materials[0].diffuse_texname.empty()) {
                    std::string texturePath = mtlBasePath + materials[0].diffuse_texname;
                    GLuint texID = loadTexture(texturePath.c_str());
                    if (texID != 0) {
                        textureIDs.push_back(texID);
                    }
                }
            }


            for (int v = 0; v < fv; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                Vertex vertex;

                vertex.Position = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };

                if (idx.normal_index >= 0) {
                    vertex.Normal = {
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    };
                }
                else {
                    vertex.Normal = { 0.0f, 0.0f, 0.0f }; // ���߼��㷨��
                }

                if (idx.texcoord_index >= 0) {
                    vertex.TexCoords = {
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * idx.texcoord_index + 1] // OBJ ͨ��V���ϣ�OpenGL����
                    };
                }
                else {
                    vertex.TexCoords = { 0.0f, 0.0f };
                }

                std::tuple<int, int, int> T(idx.vertex_index, idx.normal_index, idx.texcoord_index);

                if (uniqueVertices.count(T) == 0) {
                    uniqueVertices[T] = static_cast<unsigned int>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[T]);
            }
            index_offset += fv;
        }
    }

    if (vertices.empty()) {
        std::cerr << "No vertices loaded from " << path << std::endl;
        return false;
    }

    setupMesh();
    return true;
}

void ObjectModel::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // ����λ��
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    // ���㷨��
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // ������������
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    // glUseProgram(shaderProgram); // ͨ��������Ⱦѭ���е���

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // ������������ (����ֻ��һ����������������)
    // �������Ҫ�����ӵ��߼���������������û����������
    if (!textureIDs.empty()) {
        glActiveTexture(GL_TEXTURE0); // ��������Ԫ0
        glBindTexture(GL_TEXTURE_2D, textureIDs[0]); // �󶨴�ģ�͵�����
        // ȷ����ɫ���е� sampler2D uniform ����Ϊ0
        // glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse1"), 0); // ͨ���ڳ�ʼ��ʱ����һ��
    }


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (!textureIDs.empty()) {
        glBindTexture(GL_TEXTURE_2D, 0); // �������
    }
}

GLuint ObjectModel::loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unknown number of texture components: " << nrComponents << " for " << path << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data); // data ��ʱΪnullptr��������Ҳ�޺�
        glDeleteTextures(1, &textureID);
        return 0;
    }
    return textureID;
}