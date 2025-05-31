#include "ObjectModel.h"
#include "OurObjLoader.h" // 确保包含了 OurObjMesh 的定义
#include <iostream>
#include <set>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>


// objSplitString 和 objParseFaceVertexIndices 函数保持不变 (来自您的代码)
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
ObjectModel::~ObjectModel() { /* 析构函数实现不变 */ }
void ObjectModel::setupGpuResourcesForSubMesh(SubMesh& meshToSetup,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices) {
    
    // 设置indexCount
    meshToSetup.indexCount = static_cast<GLsizei>(indices.size());
    
    // 创建并绑定VAO
    glGenVertexArrays(1, &meshToSetup.VAO);
    glGenBuffers(1, &meshToSetup.VBO);
    glGenBuffers(1, &meshToSetup.EBO);

    glBindVertexArray(meshToSetup.VAO);

    // 绑定顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, meshToSetup.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // 绑定索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshToSetup.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针
    // 位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    // 解绑VAO
    glBindVertexArray(0);
}

bool ObjectModel::load(const std::string& objFilePath, const std::string& mtlBasePath) {
    // 1. 清理旧数据 (不变)
    for (SubMesh& sm : subMeshes) {
        if (sm.VAO != 0) glDeleteVertexArrays(1, &sm.VAO);
        if (sm.VBO != 0) glDeleteBuffers(1, &sm.VBO);
        if (sm.EBO != 0) glDeleteBuffers(1, &sm.EBO);
        sm.VAO = sm.VBO = sm.EBO = 0;
    }
    subMeshes.clear();
    for (GLuint textureID : uniqueTextureIDsForCleanup) {
        if (textureID != 0) glDeleteTextures(1, &textureID);
    }
    uniqueTextureIDsForCleanup.clear();

    // 2. 使用 OurObjLoader 加载模型数据 (不变)
    OurObjLoader loaderInstance;
    std::vector<OurObjMesh> loadedMeshesFromLoader;
    std::cout << "ObjectModel: Attempting to load model '" << objFilePath << "' with OurObjLoader." << std::endl;
    if (!loaderInstance.loadObj(objFilePath, mtlBasePath, loadedMeshesFromLoader)) {
        std::cerr << "OBJECTMODEL ERROR: OurObjLoader failed to load model: " << objFilePath << std::endl;
        return false;
    }
    if (loadedMeshesFromLoader.empty()) {
        std::cout << "ObjectModel: OurObjLoader returned no meshes for " << objFilePath << "." << std::endl;
        return true;
    }
    std::cout << "ObjectModel: OurObjLoader provided " << loadedMeshesFromLoader.size() << " mesh parts." << std::endl;

    // 3. 为每个 OurObjMesh 创建并设置 ObjectModel::SubMesh
    std::set<GLuint> tempUniqueTextureIDs;
    for (const OurObjMesh& sourceMesh : loadedMeshesFromLoader) {
        if (sourceMesh.vertices.empty() || sourceMesh.indices.empty()) {
            std::cout << "ObjectModel: Skipping an empty mesh part named '" << sourceMesh.name 
                      << "' (vertices: " << sourceMesh.vertices.size() 
                      << ", indices: " << sourceMesh.indices.size() << ")" << std::endl;
            continue;
        }

        SubMesh newSubMesh;
        newSubMesh.diffuseTextureId = sourceMesh.diffuseTextureId;
        newSubMesh.materialName = sourceMesh.materialName;
        newSubMesh.indexCount = static_cast<GLsizei>(sourceMesh.indices.size());  // 设置indexCount

        if (newSubMesh.diffuseTextureId != 0) {
            tempUniqueTextureIDs.insert(newSubMesh.diffuseTextureId);
        }

        std::vector<Vertex> modelVertices;
        modelVertices.reserve(sourceMesh.vertices.size());
        for (const auto& ov : sourceMesh.vertices) {
            modelVertices.push_back({ ov.position, ov.normal, ov.texCoords });
        }

        setupGpuResourcesForSubMesh(newSubMesh, modelVertices, sourceMesh.indices);
        subMeshes.push_back(newSubMesh);

        std::cout << "ObjectModel: Processed submesh '" << sourceMesh.name
                  << "' (Material: " << newSubMesh.materialName << ")"
                  << " with " << newSubMesh.indexCount << " indices"
                  << " and " << modelVertices.size() << " vertices"
                  << " (TextureID: " << newSubMesh.diffuseTextureId << ")" << std::endl;
    }

    // 4. 存储唯一的纹理ID以供将来清理 (不变)
    uniqueTextureIDsForCleanup.assign(tempUniqueTextureIDs.begin(), tempUniqueTextureIDs.end());
    std::cout << "ObjectModel: Successfully loaded and processed '" << objFilePath << "'." << std::endl;
    std::cout << "  Total sub-meshes: " << subMeshes.size() << std::endl;
    std::cout << "  Total unique textures for cleanup: " << uniqueTextureIDsForCleanup.size() << std::endl;

    return !subMeshes.empty();
}

void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    // 这个 draw 方法保持原样，用于绘制非水面部分或作为默认绘制方法
    glUseProgram(shaderProgram); // 确保着色器被激活

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    if (modelLoc != -1) { // 检查uniform是否存在
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    } else {
        // std::cerr << "ObjectModel::draw WARNING: 'model' uniform not found in shader " << shaderProgram << std::endl;
    }

    // 假设漫反射纹理使用纹理单元0，并且sampler名为 "texture_diffuse1"
    GLint diffuseSamplerLoc = glGetUniformLocation(shaderProgram, "texture_diffuse1");
    if (diffuseSamplerLoc != -1) {
        glUniform1i(diffuseSamplerLoc, 0);
    }


    for (const SubMesh& sm : subMeshes) {
        if (sm.VAO == 0 || sm.indexCount == 0) continue;

        if (sm.diffuseTextureId != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sm.diffuseTextureId);
        } else {
            // 如果没有漫反射纹理，可以考虑绑定一个1x1的白色纹理或不绑定
            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, 0); // 解绑
        }

        glBindVertexArray(sm.VAO);
        glDrawElements(GL_TRIANGLES, sm.indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}