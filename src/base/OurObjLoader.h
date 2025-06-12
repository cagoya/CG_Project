#pragma once
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glad/gl.h>

// 材质信息结构体
struct MaterialInfo {
    std::string name;
    glm::vec3 ambientColor = glm::vec3(0.0f);
    glm::vec3 diffuseColor = glm::vec3(0.0f);
    glm::vec3 specularColor = glm::vec3(0.0f);
    float shininess = 0.0f;
    float dissolve = 1.0f;
    std::string diffuseTextureMap;
};
// 顶点结构体
struct OurObjVertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
};
// 网格结构体
struct OurObjMesh {
    std::string name;
    std::string materialName;
    std::vector<OurObjVertex> vertices;
    std::vector<unsigned int> indices;
    GLuint diffuseTextureId = 0;
};

class OurObjLoader {
private:
    // 临时存储容器
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;
    
    // 材质和纹理缓存
    std::map<std::string, MaterialInfo> m_loadedMaterials;
    std::map<std::string, GLuint> m_textureCache;
    
    // 私有方法
    bool objParseFaceVertexIndices(const std::string& faceVertexStr, 
                                 int& v_idx, int& vt_idx, int& vn_idx,
                                 size_t max_v, size_t max_vt, size_t max_vn);
    void parseFaceVertex(const std::string& faceVertexStr, 
                        int& v_idx, int& vt_idx, int& vn_idx);
    bool parseMtlFile(const std::string& mtlFilePath,
                     std::map<std::string, MaterialInfo>& materials);
    GLuint loadTextureFromFile(const char* path);
    void processVerticesSIMD(std::vector<glm::vec3>& vertices);
    
public:
    OurObjLoader();
    ~OurObjLoader();
    
    bool loadObj(const std::string& objFilePath, 
                const std::string& mtlFileBasePath,
                std::vector<OurObjMesh>& outMeshes);
}; 