#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// 包含了所有需要的对象和工具的头文件
#include "girraffe.h"
#include "ObjectModel/ObjectModel.h"
#include "shader.h"
#include "SceneObject.h"
#include "PoolObject.h"
#include "TreeObject.h"
#include "include/base/light.h"

// 场景管理器
class SceneManager {
public:
    static SceneManager& getInstance() {
        static SceneManager instance;
        return instance;
    }

    void initialize();
    void update(float deltaTime);
    void draw(const Shader& defaultShader, const Shader& waterShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition,const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight);

    template<typename T, typename... Args>
    std::shared_ptr<T> addObject(const std::string& name, Args&&... args) {
        auto object = std::make_shared<T>(name, std::forward<Args>(args)...);
        objects_[name] = object;
        return object;
    }
    std::shared_ptr<SceneObject> getObject(const std::string& name) {
        auto it = objects_.find(name);
        if (it != objects_.end()) {
            return it->second;
        }
        return nullptr;
    }
    void clear() {
        objects_.clear();
    }
    void setAmbientLight(const AmbientLight& light) { ambientLight = light; }
    void setDirectionalLight(const DirectionalLight& light) { directionalLight = light; }
    void setSpotLight(const SpotLight& light) { spotLight = light; }

private:
    SceneManager() = default;
    ~SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<SceneObject>> objects_;

    AmbientLight ambientLight;
    DirectionalLight directionalLight;
    SpotLight spotLight;
};