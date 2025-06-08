#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
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

    void initialize() {
        // 创建游泳池对象
        auto pool = std::make_shared<PoolObject>("SwimmingPool");
        pool->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        pool->setScale(1.0f);
        pool->initialize();
        objects_["SwimmingPool"] = pool;

        // 创建树对象
        // auto tree = std::make_shared<TreeObject>("Tree");
        // tree->setPosition(glm::vec3(5.0f, 0.0f, 5.0f));
        // tree->setScale(1.0f);
        // tree->initialize();
        // objects_["Tree"] = tree;
    }

    void update(float deltaTime) {
        for (auto& [name, object] : objects_) {
            object->update(deltaTime);
        }
    }

    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection);

    // 使用模板方法添加对象
    template<typename T, typename... Args>
    std::shared_ptr<T> addObject(const std::string& name, Args&&... args) {
        auto object = std::make_shared<T>(name, std::forward<Args>(args)...);
        objects_[name] = object;
        return object;
    }

    // 获取对象
    std::shared_ptr<SceneObject> getObject(const std::string& name) {
        auto it = objects_.find(name);
        if (it != objects_.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 清理所有对象
    void clear() {
        objects_.clear();
    }

    // 设置光照参数
    void setAmbientLight(const AmbientLight& light) { ambientLight = light; }
    void setDirectionalLight(const DirectionalLight& light) { directionalLight = light; }
    void setSpotLight(const SpotLight& light) { spotLight = light; }

private:
    SceneManager() = default;
    ~SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<SceneObject>> objects_;
    
    // 光照参数
    AmbientLight ambientLight;
    DirectionalLight directionalLight;
    SpotLight spotLight;
}; 