#ifndef OUROBJLOADER_H
#define OUROBJLOADER_H

#include <string>
#include <vector>
#include <glm/glm.hpp> // ȷ�������Ŀ�Ѱ��� GLM

// ��֮ǰ����Ķ���ṹ����
struct OurObjVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// ��֮ǰ���������ṹ����
struct OurObjMesh {
    std::string name;                 // ������������
    std::string materialName;         // �� Mesh ʹ�õĲ�������
    std::vector<OurObjVertex> vertices;  // �� Mesh ������Ψһ����
    std::vector<unsigned int> indices; // ��������εĶ�������

    // ���������������� OpenGL ��صĳ�Ա������ VAO, VBO, EBO
    // GLuint VAO, VBO, EBO;
    // void setupOpenGLBuffers();
    // void draw(unsigned int shaderProgram);
};

class OurObjLoader {
public:
    OurObjLoader();
    ~OurObjLoader();

    // ���� OBJ �ļ�
    // path: .obj �ļ�·��
    // outMeshes: ������������غ��������������
    // outMaterialLibs: ����������� .obj �ļ���ͨ�� mtllib ���õĲ��ʿ��ļ����б�
    bool loadObj(const std::string& path,
        std::vector<OurObjMesh>& outMeshes,
        std::vector<std::string>& outMaterialLibs);

private:
    void parseFaceVertex(const std::string& faceVertexStr, int& v_idx, int& vt_idx, int& vn_idx);

    // ��ʱ�洢���ļ���ֱ�Ӷ�ȡ������
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;
};

#endif // OUROBJLOADER_H