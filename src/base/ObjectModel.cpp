#include "ObjectModel.h"
#include "OurObjLoader.h"
#include <iostream>
#include <set> // 用于收集唯一纹理ID
#include <glm/gtc/type_ptr.hpp>
#include <sstream>


// 辅助函数：分割字符串
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

// 辅助函数：解析面顶点字符串 
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
    // 清理所有子网格的OpenGL资源
    for (SubMesh& sm : subMeshes) {
        if (sm.VAO != 0) glDeleteVertexArrays(1, &sm.VAO);
        if (sm.VBO != 0) glDeleteBuffers(1, &sm.VBO);
        if (sm.EBO != 0) glDeleteBuffers(1, &sm.EBO);
    }
    subMeshes.clear();

    // 清理所有收集到的唯一纹理ID
    // OurObjLoader 加载了这些纹理，ObjectModel 在这里负责释放它们
    for (GLuint textureID : uniqueTextureIDsForCleanup) {
        if (textureID != 0) { // 确保ID有效
            glDeleteTextures(1, &textureID);
        }
    }
    uniqueTextureIDsForCleanup.clear();
    std::cout << "ObjectModel: Cleaned up." << std::endl;
}

void ObjectModel::setupGpuResourcesForSubMesh(SubMesh& meshToSetup,
    const std::vector<Vertex>& vertices, // 使用公共的 Vertex 结构
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

    // 顶点位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    // 顶点法线属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 顶点纹理坐标属性
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0); // 解绑VAO
    meshToSetup.indexCount = static_cast<GLsizei>(indices.size());
}


bool ObjectModel::load(const std::string& objFilePath, const std::string& mtlBasePath) {
    // 1. 清理旧数据
    for (SubMesh& sm : subMeshes) {
        if (sm.VAO != 0) glDeleteVertexArrays(1, &sm.VAO);
        if (sm.VBO != 0) glDeleteBuffers(1, &sm.VBO);
        if (sm.EBO != 0) glDeleteBuffers(1, &sm.EBO);
        sm.VAO = sm.VBO = sm.EBO = 0; // 重置ID
    }
    subMeshes.clear();

    for (GLuint textureID : uniqueTextureIDsForCleanup) {
        if (textureID != 0) glDeleteTextures(1, &textureID);
    }
    uniqueTextureIDsForCleanup.clear();

    // 2. 使用 OurObjLoader 加载模型数据
    OurObjLoader loaderInstance; // 创建加载器实例
    std::vector<OurObjMesh> loadedMeshesFromLoader; // OurObjLoader.h 中定义的结构

    std::cout << "ObjectModel: Attempting to load model '" << objFilePath << "' with OurObjLoader." << std::endl;
    if (!loaderInstance.loadObj(objFilePath, mtlBasePath, loadedMeshesFromLoader)) {
        std::cerr << "OBJECTMODEL ERROR: OurObjLoader failed to load model: " << objFilePath << std::endl;
        return false;
    }

    if (loadedMeshesFromLoader.empty()) {
        std::cout << "ObjectModel: OurObjLoader returned no meshes for " << objFilePath << "." << std::endl;
        // 这不一定是错误，可能是空模型，但通常模型至少有一个网格
        return true; // 或者 false，取决于如何定义“成功加载空模型”
    }

    std::cout << "ObjectModel: OurObjLoader provided " << loadedMeshesFromLoader.size() << " mesh parts." << std::endl;

    // 3. 为每个 OurObjMesh 创建并设置 ObjectModel::SubMesh
    std::set<GLuint> tempUniqueTextureIDs; // 用于收集此加载操作中的唯一纹理ID

    for (const OurObjMesh& sourceMesh : loadedMeshesFromLoader) {
        if (sourceMesh.vertices.empty() || sourceMesh.indices.empty()) {
            std::cout << "ObjectModel: Skipping an empty mesh part named '" << sourceMesh.name << "' from OurObjLoader." << std::endl;
            continue;
        }

        SubMesh newSubMesh;
        newSubMesh.diffuseTextureId = sourceMesh.diffuseTextureId; // 直接使用加载器提供的纹理ID

        if (newSubMesh.diffuseTextureId != 0) {
            tempUniqueTextureIDs.insert(newSubMesh.diffuseTextureId);
        }

        // 将 OurObjVertex 转换为 ObjectModel::Vertex (如果它们不完全相同)
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

    // 4. 存储唯一的纹理ID以供将来清理
    uniqueTextureIDsForCleanup.assign(tempUniqueTextureIDs.begin(), tempUniqueTextureIDs.end());

    //std::cout << "ObjectModel: Successfully loaded and processed '" << objFilePath << "'." << std::endl;
    //std::cout << "  Total sub-meshes: " << subMeshes.size() << std::endl;
    //std::cout << "  Total unique textures for cleanup: " << uniqueTextureIDsForCleanup.size() << std::endl;

    return !subMeshes.empty(); // 成功加载的标准是至少有一个有效的子网格
}

void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    // 设置模型变换矩阵 (通常一次即可，除非子网格有不同变换)
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // 通常，纹理单元的uniform在着色器初始化时设置一次，例如：
    // glUseProgram(shaderProgram);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse1"), 0); // 漫反射纹理使用单元0

    for (const SubMesh& sm : subMeshes) {
        if (sm.VAO == 0 || sm.indexCount == 0) continue; // 跳过无效或空的子网格

        // 绑定该子网格的漫反射纹理 (如果存在)
        if (sm.diffuseTextureId != 0) {
            glActiveTexture(GL_TEXTURE0); // 激活纹理单元0 (或其他指定的单元)
            glBindTexture(GL_TEXTURE_2D, sm.diffuseTextureId);
        }
        else {
            // 如果没有纹理，可以选择绑定一个默认纹理或确保着色器能处理无纹理情况
            //glBindTexture(GL_TEXTURE_2D, 0); // 解绑任何先前绑定的纹理（如果需要）
        }

        // 绑定并绘制子网格
        glBindVertexArray(sm.VAO);
        glDrawElements(GL_TRIANGLES, sm.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // 绘制完毕后解绑VAO是个好习惯
    }
    // 可选：绘制完所有子网格后，解绑纹理
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, 0);
}