#include "SwimmingPool.h"
#include "base/shader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "base/PathHelper.h"
// 独立的纹理加载函数
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture FAILED to load at path: " << path << " (stb_image error: " << stbi_failure_reason() << ")" << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    return textureID;
}

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
            waterShader.setVec3("spotLight.position", spotLight.position);
            waterShader.setFloat("spotLight.intensity", spotLight.intensity);
            waterShader.setVec3("spotLight.direction", spotLight.direction);
            waterShader.setVec3("spotLight.color", spotLight.color);
            // 绑定纹理
            glActiveTexture(GL_TEXTURE0);
            waterShader.setInt("diffuseTexture", 0);
            glBindTexture(GL_TEXTURE_2D, mesh.diffuseTextureId);

            glActiveTexture(GL_TEXTURE1);
            waterShader.setInt("normalMap", 1);
            glBindTexture(GL_TEXTURE_2D, waterNormalMapID_);
        } else {
            defaultShader.use();
            defaultShader.setMat4("model", modelMatrix);
            defaultShader.setMat4("view", view);
            defaultShader.setMat4("projection", projection);
            if (glGetUniformLocation(defaultShader.id_, "normalMatrix") != -1) {
                defaultShader.setMat3("normalMatrix", normalMatrix);
            }
            defaultShader.setVec3("viewPos", cameraPosition);
            defaultShader.setVec3("ambientLight.color", ambientLight.color);
            defaultShader.setFloat("ambientLight.intensity", ambientLight.intensity);
            defaultShader.setVec3("directionalLight.direction", directionalLight.direction);
            defaultShader.setVec3("directionalLight.color", directionalLight.color);
            defaultShader.setFloat("directionalLight.intensity", directionalLight.intensity);
            defaultShader.setVec3("spotLight.position", spotLight.position);
            defaultShader.setVec3("spotLight.direction", spotLight.direction);
            glActiveTexture(GL_TEXTURE0);
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