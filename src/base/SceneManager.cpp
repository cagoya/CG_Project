#include "SceneManager.h"

void SceneManager::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    for (auto& [name, object] : objects_) {
        // 设置光照参数
        shader.setVec3("ambientColor", ambientLight.color);
        shader.setFloat("ambientIntensity", ambientLight.intensity);
        
        // 方向光
        shader.setVec3("directionalLight.direction", directionalLight.direction);
        shader.setVec3("directionalLight.color", directionalLight.color);
        shader.setFloat("directionalLight.intensity", directionalLight.intensity);
        
        // 聚光灯
        shader.setVec3("spotLight.position", spotLight.position);
        shader.setVec3("spotLight.direction", spotLight.direction);
        shader.setVec3("spotLight.color", spotLight.color);
        shader.setFloat("spotLight.intensity", spotLight.intensity);
        shader.setFloat("spotLight.angle", spotLight.angle);
        shader.setFloat("spotLight.kc", spotLight.kc);
        shader.setFloat("spotLight.kl", spotLight.kl);
        shader.setFloat("spotLight.kq", spotLight.kq);

        object->draw(shader, view, projection);
    }
} 