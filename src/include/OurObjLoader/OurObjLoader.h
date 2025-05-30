#ifndef OUROBJLOADER_H
#define OUROBJLOADER_H

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glad/gl.h>

// 结构体：存储从 MTL 文件解析的材质属性
struct MaterialInfo {
    std::string name;
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;
    float dissolve = 1.0f;
    std::string diffuseTextureMap; // map_Kd
    // 你可以添加其他贴图属性
    // std::string specularTextureMap;
    // std::string bumpTextureMap;

    MaterialInfo() = default;
};

// 结构体：单个顶点的数据
struct OurObjVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// 结构体：表示模型中的一个网格
struct OurObjMesh {
    std::string name;
    std::string materialName;
    std::vector<OurObjVertex> vertices;
    std::vector<unsigned int> indices;
    GLuint diffuseTextureId = 0; // 新增：存储此网格使用的漫反射纹理的OpenGL ID

    // 你可能还想在这里存储 VAO, VBO, EBO
    // GLuint VAO = 0, VBO = 0, EBO = 0;
};

class OurObjLoader {
public:
    OurObjLoader();
    ~OurObjLoader();

    /**
     * 加载 .obj 模型文件，并解析其引用的 .mtl 材质文件，同时加载漫反射纹理。
     * @param objFilePath .obj 文件的路径。
     * @param mtlFileBasePath 用于查找 .mtl 文件以及 .mtl 文件内部引用的纹理文件的基础目录路径。
     * @param outMeshes 一个 std::vector<OurObjMesh> 的引用，用于存储加载和解析出的所有网格数据。
     * @return 如果成功加载和解析 OBJ 及关联的 MTL 文件和主要纹理，则返回 true，否则返回 false。
     */
    bool loadObj(const std::string& objFilePath,
        const std::string& mtlFileBasePath,
        std::vector<OurObjMesh>& outMeshes);

private:
    void parseFaceVertex(const std::string& faceVertexStr, int& v_idx, int& vt_idx, int& vn_idx);
    bool parseMtlFile(const std::string& mtlFilePath,
        std::map<std::string, MaterialInfo>& materials);
    std::string joinPaths(const std::string& basePath, const std::string& relativePath);
    // 内部使用，填充 m_loadedMaterials
    bool objParseFaceVertexIndices(const std::string& faceVertexStr,
        int& v_idx, int& vt_idx, int& vn_idx,
        size_t max_v, size_t max_vt, size_t max_vn);
    GLuint loadTextureFromFile(const char* path);

    // 成员变量存储解析结果和缓存
    std::map<std::string, MaterialInfo> m_loadedMaterials;
    std::map<std::string, GLuint> m_textureCache; // 缓存已加载的纹理，避免重复加载

    // 临时存储从 .obj 文件中直接读取的原始几何数据
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;
};

#endif // OUROBJLOADER_H