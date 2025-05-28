#ifndef OUROBJLOADER_H
#define OUROBJLOADER_H

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glad/gl.h>

// �ṹ�壺�洢�� MTL �ļ������Ĳ�������
struct MaterialInfo {
    std::string name;
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;
    float dissolve = 1.0f;
    std::string diffuseTextureMap; // map_Kd
    // ��������������ͼ����
    // std::string specularTextureMap;
    // std::string bumpTextureMap;

    MaterialInfo() = default;
};

// �ṹ�壺�������������
struct OurObjVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// �ṹ�壺��ʾģ���е�һ������
struct OurObjMesh {
    std::string name;
    std::string materialName;
    std::vector<OurObjVertex> vertices;
    std::vector<unsigned int> indices;
    GLuint diffuseTextureId = 0; // �������洢������ʹ�õ������������OpenGL ID

    // ����ܻ���������洢 VAO, VBO, EBO
    // GLuint VAO = 0, VBO = 0, EBO = 0;
};

class OurObjLoader {
public:
    OurObjLoader();
    ~OurObjLoader();

    /**
     * ���� .obj ģ���ļ��������������õ� .mtl �����ļ���ͬʱ��������������
     * @param objFilePath .obj �ļ���·����
     * @param mtlFileBasePath ���ڲ��� .mtl �ļ��Լ� .mtl �ļ��ڲ����õ������ļ��Ļ���Ŀ¼·����
     * @param outMeshes һ�� std::vector<OurObjMesh> �����ã����ڴ洢���غͽ������������������ݡ�
     * @return ����ɹ����غͽ��� OBJ �������� MTL �ļ�����Ҫ�����򷵻� true�����򷵻� false��
     */
    bool loadObj(const std::string& objFilePath,
        const std::string& mtlFileBasePath,
        std::vector<OurObjMesh>& outMeshes);

private:
    void parseFaceVertex(const std::string& faceVertexStr, int& v_idx, int& vt_idx, int& vn_idx);
    bool parseMtlFile(const std::string& mtlFilePath,
        std::map<std::string, MaterialInfo>& materials);
    std::string joinPaths(const std::string& basePath, const std::string& relativePath);
    // �ڲ�ʹ�ã���� m_loadedMaterials
    bool objParseFaceVertexIndices(const std::string& faceVertexStr,
        int& v_idx, int& vt_idx, int& vn_idx,
        size_t max_v, size_t max_vt, size_t max_vn);
    GLuint loadTextureFromFile(const char* path);

    // ��Ա�����洢��������ͻ���
    std::map<std::string, MaterialInfo> m_loadedMaterials;
    std::map<std::string, GLuint> m_textureCache; // �����Ѽ��ص����������ظ�����

    // ��ʱ�洢�� .obj �ļ���ֱ�Ӷ�ȡ��ԭʼ��������
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;
};

#endif // OUROBJLOADER_H