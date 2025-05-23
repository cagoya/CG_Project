#ifndef OUROBJLOADER_H
#define OUROBJLOADER_H

#include <string>
#include <vector>
#include <glm/glm.hpp> // 确保你的项目已包含 GLM

// 和之前定义的顶点结构类似
struct OurObjVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// 和之前定义的网格结构类似
struct OurObjMesh {
    std::string name;                 // 对象或组的名称
    std::string materialName;         // 此 Mesh 使用的材质名称
    std::vector<OurObjVertex> vertices;  // 此 Mesh 的所有唯一顶点
    std::vector<unsigned int> indices; // 组成三角形的顶点索引

    // 你可以在这里添加与 OpenGL 相关的成员，例如 VAO, VBO, EBO
    // GLuint VAO, VBO, EBO;
    // void setupOpenGLBuffers();
    // void draw(unsigned int shaderProgram);
};

class OurObjLoader {
public:
    OurObjLoader();
    ~OurObjLoader();

    // 加载 OBJ 文件
    // path: .obj 文件路径
    // outMeshes: 输出参数，加载后的所有网格数据
    // outMaterialLibs: 输出参数，在 .obj 文件中通过 mtllib 引用的材质库文件名列表
    bool loadObj(const std::string& path,
        std::vector<OurObjMesh>& outMeshes,
        std::vector<std::string>& outMaterialLibs);

private:
    void parseFaceVertex(const std::string& faceVertexStr, int& v_idx, int& vt_idx, int& vn_idx);

    // 临时存储从文件中直接读取的数据
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;
};

#endif // OUROBJLOADER_H