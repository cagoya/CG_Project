#include "SwimmingPool.h"
#include "base/shader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "base/PathHelper.h"
// 这是一个独立的纹理加载函数，你需要它来加载法线贴图
// (这段代码来自我们之前的讨论，确保它在这里或者一个可被包含的工具文件中)
// 在 SwimmingPool.cpp 文件顶部
// 在 SwimmingPool.cpp 文件顶部
GLuint loadTextureFromFile(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // glGenerateMipmap(GL_TEXTURE_2D); // <-- 注释掉 Mipmap 生成

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // <-- 使用不需要 Mipmap 的过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Texture loaded successfully from path: " << path << std::endl;
    } else {
        std::cout << "Texture FAILED to load at path: " << path << " (stb_image error: " << stbi_failure_reason() << ")" << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    return textureID;
}

// --- SwimmingPool 类的实现 ---

SwimmingPool::SwimmingPool() 
    : position_(5.5f, 0.4f, 2.0f), scale_(0.09f) {}

SwimmingPool::~SwimmingPool() {
    delete model_;
}

bool SwimmingPool::initialize(const std::string& normalMapPath) {
    model_ = new ObjectModel();
    std::string objPath = PathHelper::get("media/model/new_pool/source/swimmingPool.obj");
    std::string mtlPath = PathHelper::get("media/model/new_pool/source/");
    
    if (!model_->load(objPath, mtlPath)) {
        std::cerr << "Failed to load swimming pool model!" << std::endl;
        delete model_;
        model_ = nullptr;
        return false;
    }
    std::cout << "Successfully loaded swimming pool model!" << std::endl;

    waterNormalMapID_ = loadTextureFromFile(normalMapPath.c_str());
    if (waterNormalMapID_ == 0) {
        std::cerr << "Failed to load water normal map!" << std::endl;
        return false;
    }
    return true;
}

void SwimmingPool::update(float deltaTime) {
    time_ += deltaTime;
}

// 在 SwimmingPool.cpp 中

// +++ 这是 draw 函数的最终版本 +++
void SwimmingPool::draw(const Shader& defaultShader, const Shader& waterShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition, const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    if (!model_) return;

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position_) * glm::scale(glm::mat4(1.0f), glm::vec3(scale_));
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

    for (const auto& mesh : model_->getSubMeshes()) {

        if (mesh.materialName == "water") {
            // --- 如果是水，使用 waterShader ---
            waterShader.use();
            waterShader.setMat4("model", modelMatrix);
            waterShader.setMat4("view", view);
            waterShader.setMat4("projection", projection);
            waterShader.setMat3("normalMatrix", normalMatrix);
            waterShader.setFloat("time", time_);
            waterShader.setVec3("viewPos", cameraPosition);

            // 为水着色器设置完整的光照
            waterShader.setVec3("ambientLight.color", ambientLight.color);
            waterShader.setFloat("ambientLight.intensity", ambientLight.intensity);
            waterShader.setVec3("directionalLight.direction", directionalLight.direction);
            waterShader.setVec3("directionalLight.color", directionalLight.color);
            waterShader.setFloat("directionalLight.intensity", directionalLight.intensity);
            // 如果你的 water.frag 需要聚光灯，也可以在这里添加 spotLight 的 uniforms

            // 绑定纹理
            glActiveTexture(GL_TEXTURE0);
            waterShader.setInt("diffuseTexture", 0);
            glBindTexture(GL_TEXTURE_2D, mesh.diffuseTextureId);

            glActiveTexture(GL_TEXTURE1);
            waterShader.setInt("normalMap", 1);
            glBindTexture(GL_TEXTURE_2D, waterNormalMapID_);
        } else {
            // --- 如果是其他部分（瓷砖等），使用默认着色器 ---
            defaultShader.use();
            defaultShader.setMat4("model", modelMatrix);
            defaultShader.setMat4("view", view);
            defaultShader.setMat4("projection", projection);

            // 确保你的默认着色器也有 normalMatrix，以保证缩放时光照正确
            if (glGetUniformLocation(defaultShader.id_, "normalMatrix") != -1) {
                defaultShader.setMat3("normalMatrix", normalMatrix);
            }

            // ++ 为默认着色器也设置完整的光照 ++
            // 注意：这里的 uniform 变量名需要和你 objModelShader 中的定义完全一致
            defaultShader.setVec3("viewPos", cameraPosition);
            defaultShader.setVec3("ambientLight.color", ambientLight.color);
            defaultShader.setFloat("ambientLight.intensity", ambientLight.intensity);
            defaultShader.setVec3("directionalLight.direction", directionalLight.direction);
            defaultShader.setVec3("directionalLight.color", directionalLight.color);
            defaultShader.setFloat("directionalLight.intensity", directionalLight.intensity);
            defaultShader.setVec3("spotLight.position", spotLight.position);
            defaultShader.setVec3("spotLight.direction", spotLight.direction);
            // ... 等等，设置所有 objModelShader 需要的光照 uniform

            // 绑定它的纹理
            glActiveTexture(GL_TEXTURE0);
            // 注意：你的 objModelShader 的纹理采样器名叫 "material.diffuse"
            defaultShader.setInt("material.diffuse", 0); 
            glBindTexture(GL_TEXTURE_2D, mesh.diffuseTextureId);
        }

        // 绑定并绘制当前的网格
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void SwimmingPool::setPosition(const glm::vec3& position) {
    position_ = position;
}

void SwimmingPool::setScale(float scale) {
    scale_ = scale;
}