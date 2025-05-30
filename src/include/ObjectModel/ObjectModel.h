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
    // 内部结构，用于表示模型的每个可独立渲染的子部分
    struct SubMesh {
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;
        GLsizei indexCount = 0;
        GLuint diffuseTextureId = 0; // 从 OurObjMesh 获取
        // 可选: 如果需要更复杂的材质属性，可以在这里存储 MaterialInfo
    };

    std::vector<SubMesh> subMeshes;
    std::vector<GLuint> uniqueTextureIDsForCleanup; // 存储所有子网格使用的唯一纹理ID，用于析构时清理

    // 辅助函数，为给定的顶点和索引数据设置OpenGL缓冲
    // vertices 和 indices 参数是 OurObjLoader 加载的数据
    //  Vertex 结构体与 OurObjLoader 中的 OurObjVertex 兼容
    void setupGpuResourcesForSubMesh(SubMesh& meshToSetup,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices);
};

#endif // OBJECTMODEL_H