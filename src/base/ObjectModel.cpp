#include "ObjectModel.h"
#include "base/OurObjLoader.h"
#include <iostream>
#include <set>
#include <glad/gl.h>

// 构造函数
ObjectModel::ObjectModel() {}

// 析构函数：释放所有GPU资源
ObjectModel::~ObjectModel() {
    for (const auto& mesh : subMeshes) {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }
    for (GLuint textureId : uniqueTextureIDsForCleanup) {
        if(textureId != 0) {
            glDeleteTextures(1, &textureId);
        }
    }
}

// 加载模型的实现
bool ObjectModel::load(const std::string& path, const std::string& mtlBasePath) {
    OurObjLoader loader;
    std::vector<OurObjMesh> loadedMeshes; // 用于接收加载器返回的数据

    // 1. 从文件加载数据到内存
    if (!loader.loadObj(path, mtlBasePath, loadedMeshes)) {
        return false;
    }

    std::set<GLuint> uniqueTextures;

    // 2. 遍历加载好的每个模型部件
    for (const auto& loaderMesh : loadedMeshes) {
        if (loaderMesh.vertices.empty() || loaderMesh.indices.empty()) {
            continue; // 跳过空部件
        }

        // --- 这是最关键的数据转换步骤 ---
        // 将 OurObjLoader 的顶点结构 转换为 ObjectModel 的 Vertex 结构
        std::vector<Vertex> verticesForGpu;
        verticesForGpu.reserve(loaderMesh.vertices.size());
        for(const auto& v_from_loader : loaderMesh.vertices){
            Vertex v_to_gpu;
            v_to_gpu.Position = v_from_loader.position;
            v_to_gpu.Normal = v_from_loader.normal;
            v_to_gpu.TexCoords = v_from_loader.texCoords; // <-- 确保这一行正确无误
            verticesForGpu.push_back(v_to_gpu);
        }
        // ------------------------------------

        SubMesh newSubMesh;

        // 3. 为这个部件准备GPU资源 (VAO/VBO/EBO)，并将转换好的数据上传
        setupGpuResourcesForSubMesh(newSubMesh, verticesForGpu, loaderMesh.indices);

        // 4. 复制其他信息
        newSubMesh.materialName = loaderMesh.materialName;
        newSubMesh.diffuseTextureId = loaderMesh.diffuseTextureId;

        this->subMeshes.push_back(newSubMesh);

        if (newSubMesh.diffuseTextureId != 0) {
            uniqueTextures.insert(newSubMesh.diffuseTextureId);
        }
    }

    this->uniqueTextureIDsForCleanup.assign(uniqueTextures.begin(), uniqueTextures.end());
    return true;
}

// 为单个 SubMesh 配置 VAO/VBO/EBO 的实现
void ObjectModel::setupGpuResourcesForSubMesh(SubMesh& meshToSetup,
                                              const std::vector<Vertex>& vertices,
                                              const std::vector<unsigned int>& indices) {
    meshToSetup.indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &meshToSetup.VAO);
    glGenBuffers(1, &meshToSetup.VBO);
    glGenBuffers(1, &meshToSetup.EBO);

    glBindVertexArray(meshToSetup.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, meshToSetup.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshToSetup.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 位置属性 (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    // 法线属性 (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 纹理坐标属性 (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

// 遗留的 draw 方法的简单实现
void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelMatrix[0][0]);

    for (const auto& mesh : subMeshes) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.diffuseTextureId);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
// 在 ObjectModel.cpp 中
void ObjectModel::simpleDraw() {
    for (const auto& mesh : subMeshes) {
        glBindVertexArray(mesh.VAO);
        // 注意：这里不再绑定纹理，因为纹理绑定应该由具体的对象（如AnimatedDog）或更上层的逻辑处理
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}