#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <string>
#include <vector>
#include <glad/gl.h> 
#include <glm/glm.hpp>

// ���嶥��ṹ������λ�á����ߺ���������
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class ObjectModel {
public:
    ObjectModel();
    ~ObjectModel();

    // ���� OBJ ģ�ͣ�mtlBasePath �� .mtl �ļ�������ͼƬ���ڵ�Ŀ¼
    bool load(const std::string& path, const std::string& mtlBasePath);
    void draw(unsigned int shaderProgram, const glm::mat4& modelMatrix);

private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<GLuint> textureIDs; // ���ص�����ID
    void setupMesh();
    GLuint loadTexture(const char* path); // ����������������
};

#endif // OBJECTMODEL_H