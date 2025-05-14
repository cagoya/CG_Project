#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <string>
#include <vector>
#include <glad/gl.h> 
#include <glm/glm.hpp>

// 定义顶点结构，包含位置、法线和纹理坐标
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class ObjectModel {
public:
    ObjectModel();
    ~ObjectModel();

    // 加载 OBJ 模型，mtlBasePath 是 .mtl 文件和纹理图片所在的目录
    bool load(const std::string& path, const std::string& mtlBasePath);
    void draw(unsigned int shaderProgram, const glm::mat4& modelMatrix);

private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<GLuint> textureIDs; // 加载的纹理ID
    void setupMesh();
    GLuint loadTexture(const char* path); // 辅助函数加载纹理
};

#endif // OBJECTMODEL_H