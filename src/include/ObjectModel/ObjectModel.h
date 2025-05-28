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
    // �ڲ��ṹ�����ڱ�ʾģ�͵�ÿ���ɶ�����Ⱦ���Ӳ���
    struct SubMesh {
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;
        GLsizei indexCount = 0;
        GLuint diffuseTextureId = 0; // �� OurObjMesh ��ȡ
        // ��ѡ: �����Ҫ�����ӵĲ������ԣ�����������洢 MaterialInfo
    };

    std::vector<SubMesh> subMeshes;
    std::vector<GLuint> uniqueTextureIDsForCleanup; // �洢����������ʹ�õ�Ψһ����ID����������ʱ����

    // ����������Ϊ�����Ķ����������������OpenGL����
    // vertices �� indices ������ OurObjLoader ���ص�����
    //  Vertex �ṹ���� OurObjLoader �е� OurObjVertex ����
    void setupGpuResourcesForSubMesh(SubMesh& meshToSetup,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices);
};

#endif // OBJECTMODEL_H