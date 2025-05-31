#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <string>
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>

// 假设 OurObjLoader.h 定义了 MaterialInfo (如果 SubMesh 要存储它)
// #include "OurObjLoader.h" // 或者一个包含 MaterialInfo 的公共头文件

// 定义顶点结构
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

// 将SubMesh结构体移到类外部
struct SubMesh {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLsizei indexCount = 0;
    GLuint diffuseTextureId = 0;
    std::string materialName;
};

class ObjectModel {
public:
    ObjectModel();
    virtual ~ObjectModel(); // <--- 声明为虚析构函数

    bool load(const std::string& path, const std::string& mtlBasePath);
    // 保持原来的 draw 签名，SwimmingPoolModel 将用自己的 draw 方法
    // 或者也可以将这个 draw 声明为 virtual，如果 SwimmingPoolModel 要覆盖它
    virtual void draw(unsigned int shaderProgram, const glm::mat4& modelMatrix);

    // 在类内部直接使用SubMesh类型
    const std::vector<SubMesh>& getSubMeshes() const { return subMeshes; }

protected: // <--- 改为 protected，以便派生类访问
    std::vector<SubMesh> subMeshes;
    std::vector<GLuint> uniqueTextureIDsForCleanup;

    void setupGpuResourcesForSubMesh(SubMesh& meshToSetup,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices);
};

#endif // OBJECTMODEL_H