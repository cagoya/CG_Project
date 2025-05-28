#include "ObjectModel.h"
#include "OurObjLoader.h"
#include <iostream>
#include <set> // �����ռ�Ψһ����ID
#include <glm/gtc/type_ptr.hpp>
#include <sstream>


// �����������ָ��ַ���
static inline std::vector<std::string> objSplitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// ���������������涥���ַ��� 
static inline bool objParseFaceVertexIndices(const std::string& faceVertexStr,
    int& v_idx, int& vt_idx, int& vn_idx,
    int max_v_idx, int max_vt_idx, int max_vn_idx) {
    v_idx = vt_idx = vn_idx = -1;
    std::vector<std::string> parts = objSplitString(faceVertexStr, '/');
    if (parts.empty() || parts[0].empty()) return false;
    try {
        v_idx = std::stoi(parts[0]) - 1;
        if (v_idx < 0 || v_idx >= max_v_idx) return false;
        if (parts.size() > 1 && !parts[1].empty()) {
            vt_idx = std::stoi(parts[1]) - 1;
            if (vt_idx < 0 || vt_idx >= max_vt_idx) vt_idx = -1;
        }
        if (parts.size() > 2 && !parts[2].empty()) {
            vn_idx = std::stoi(parts[2]) - 1;
            if (vn_idx < 0 || vn_idx >= max_vn_idx) vn_idx = -1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "OBJECTMODEL (helper) WARNING: Exception parsing face component '" << faceVertexStr << "': " << e.what() << std::endl;
        return false;
    }
    return true;
}


ObjectModel::ObjectModel() {}

ObjectModel::~ObjectModel() {
    // ���������������OpenGL��Դ
    for (SubMesh& sm : subMeshes) {
        if (sm.VAO != 0) glDeleteVertexArrays(1, &sm.VAO);
        if (sm.VBO != 0) glDeleteBuffers(1, &sm.VBO);
        if (sm.EBO != 0) glDeleteBuffers(1, &sm.EBO);
    }
    subMeshes.clear();

    // ���������ռ�����Ψһ����ID
    // OurObjLoader ��������Щ����ObjectModel �����︺���ͷ�����
    for (GLuint textureID : uniqueTextureIDsForCleanup) {
        if (textureID != 0) { // ȷ��ID��Ч
            glDeleteTextures(1, &textureID);
        }
    }
    uniqueTextureIDsForCleanup.clear();
    std::cout << "ObjectModel: Cleaned up." << std::endl;
}

void ObjectModel::setupGpuResourcesForSubMesh(SubMesh& meshToSetup,
    const std::vector<Vertex>& vertices, // ʹ�ù����� Vertex �ṹ
    const std::vector<unsigned int>& indices) {
    if (vertices.empty() || indices.empty()) {
        std::cerr << "OBJECTMODEL ERROR: Attempted to setup GPU resources for submesh with no vertex or index data." << std::endl;
        return;
    }

    glGenVertexArrays(1, &meshToSetup.VAO);
    glGenBuffers(1, &meshToSetup.VBO);
    glGenBuffers(1, &meshToSetup.EBO);

    glBindVertexArray(meshToSetup.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, meshToSetup.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshToSetup.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // ����λ������
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    // ���㷨������
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // ����������������
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0); // ���VAO
    meshToSetup.indexCount = static_cast<GLsizei>(indices.size());
}


bool ObjectModel::load(const std::string& objFilePath, const std::string& mtlBasePath) {
    // 1. ���������
    for (SubMesh& sm : subMeshes) {
        if (sm.VAO != 0) glDeleteVertexArrays(1, &sm.VAO);
        if (sm.VBO != 0) glDeleteBuffers(1, &sm.VBO);
        if (sm.EBO != 0) glDeleteBuffers(1, &sm.EBO);
        sm.VAO = sm.VBO = sm.EBO = 0; // ����ID
    }
    subMeshes.clear();

    for (GLuint textureID : uniqueTextureIDsForCleanup) {
        if (textureID != 0) glDeleteTextures(1, &textureID);
    }
    uniqueTextureIDsForCleanup.clear();

    // 2. ʹ�� OurObjLoader ����ģ������
    OurObjLoader loaderInstance; // ����������ʵ��
    std::vector<OurObjMesh> loadedMeshesFromLoader; // OurObjLoader.h �ж���Ľṹ

    std::cout << "ObjectModel: Attempting to load model '" << objFilePath << "' with OurObjLoader." << std::endl;
    if (!loaderInstance.loadObj(objFilePath, mtlBasePath, loadedMeshesFromLoader)) {
        std::cerr << "OBJECTMODEL ERROR: OurObjLoader failed to load model: " << objFilePath << std::endl;
        return false;
    }

    if (loadedMeshesFromLoader.empty()) {
        std::cout << "ObjectModel: OurObjLoader returned no meshes for " << objFilePath << "." << std::endl;
        // �ⲻһ���Ǵ��󣬿����ǿ�ģ�ͣ���ͨ��ģ��������һ������
        return true; // ���� false��ȡ������ζ��塰�ɹ����ؿ�ģ�͡�
    }

    std::cout << "ObjectModel: OurObjLoader provided " << loadedMeshesFromLoader.size() << " mesh parts." << std::endl;

    // 3. Ϊÿ�� OurObjMesh ���������� ObjectModel::SubMesh
    std::set<GLuint> tempUniqueTextureIDs; // �����ռ��˼��ز����е�Ψһ����ID

    for (const OurObjMesh& sourceMesh : loadedMeshesFromLoader) {
        if (sourceMesh.vertices.empty() || sourceMesh.indices.empty()) {
            std::cout << "ObjectModel: Skipping an empty mesh part named '" << sourceMesh.name << "' from OurObjLoader." << std::endl;
            continue;
        }

        SubMesh newSubMesh;
        newSubMesh.diffuseTextureId = sourceMesh.diffuseTextureId; // ֱ��ʹ�ü������ṩ������ID

        if (newSubMesh.diffuseTextureId != 0) {
            tempUniqueTextureIDs.insert(newSubMesh.diffuseTextureId);
        }

        // �� OurObjVertex ת��Ϊ ObjectModel::Vertex (������ǲ���ȫ��ͬ)
        std::vector<Vertex> modelVertices;
        modelVertices.reserve(sourceMesh.vertices.size());
        for (const auto& ov : sourceMesh.vertices) {
            modelVertices.push_back({ ov.position, ov.normal, ov.texCoords });
        }

        setupGpuResourcesForSubMesh(newSubMesh, modelVertices, sourceMesh.indices);
        subMeshes.push_back(newSubMesh);
        //std::cout << "ObjectModel: Processed submesh for '" << sourceMesh.name
            //<< "' with " << newSubMesh.indexCount << " indices and TextureID: " << newSubMesh.diffuseTextureId << std::endl;
    }

    // 4. �洢Ψһ������ID�Թ���������
    uniqueTextureIDsForCleanup.assign(tempUniqueTextureIDs.begin(), tempUniqueTextureIDs.end());

    //std::cout << "ObjectModel: Successfully loaded and processed '" << objFilePath << "'." << std::endl;
    //std::cout << "  Total sub-meshes: " << subMeshes.size() << std::endl;
    //std::cout << "  Total unique textures for cleanup: " << uniqueTextureIDsForCleanup.size() << std::endl;

    return !subMeshes.empty(); // �ɹ����صı�׼��������һ����Ч��������
}

void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    // ����ģ�ͱ任���� (ͨ��һ�μ��ɣ������������в�ͬ�任)
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // ͨ��������Ԫ��uniform����ɫ����ʼ��ʱ����һ�Σ����磺
    // glUseProgram(shaderProgram);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse1"), 0); // ����������ʹ�õ�Ԫ0

    for (const SubMesh& sm : subMeshes) {
        if (sm.VAO == 0 || sm.indexCount == 0) continue; // ������Ч��յ�������

        // �󶨸������������������ (�������)
        if (sm.diffuseTextureId != 0) {
            glActiveTexture(GL_TEXTURE0); // ��������Ԫ0 (������ָ���ĵ�Ԫ)
            glBindTexture(GL_TEXTURE_2D, sm.diffuseTextureId);
        }
        else {
            // ���û����������ѡ���һ��Ĭ�������ȷ����ɫ���ܴ������������
            //glBindTexture(GL_TEXTURE_2D, 0); // ����κ���ǰ�󶨵����������Ҫ��
        }

        // �󶨲�����������
        glBindVertexArray(sm.VAO);
        glDrawElements(GL_TRIANGLES, sm.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // ������Ϻ���VAO�Ǹ���ϰ��
    }
    // ��ѡ������������������󣬽������
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, 0);
}