#include "base/SceneManager.h" // 确保包含了正确的头文件路径
#include "base/PathHelper.h"

// 在 SceneManager.cpp 中
#include <sstream>  // For std::stringstream
#include <iomanip>  // For std::setw and std::setfill
#include "AnimatedDog.h" // 确保包含了小狗的头文件

void SceneManager::initialize() {
    // --- 创建游泳池对象 ---
    auto pool = std::make_shared<PoolObject>("SwimmingPool");
    pool->setPosition(glm::vec3(9.5f, 0.2f, 0.0f));
    pool->setScale(0.007f);
    pool->initialize(PathHelper::get("media/textures/Water004_1K_Normal.jpg"));
    objects_["SwimmingPool"] = pool;

    std::cout << "Creating animated puppy..." << std::endl;
    // 1. 定义小狗模型的基础路径
    std::string dogModelBasePath = PathHelper::get("media/model/mycutedog/");
    // 2. 动态生成所有帧的文件路径
    std::vector<std::string> puppyFramePaths;
    const int startFrame = 1;
    const int endFrame = 250; // 假设你有60帧，请根据实际情况修改
    for (int i = startFrame; i <= endFrame; ++i) {
        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << i;
        std::string frameNumber = ss.str();
        std::string frameFile = "puppy" + frameNumber + ".obj";
        puppyFramePaths.push_back(dogModelBasePath + frameFile);
    }
    // 3. 创建 AnimatedDog 实例
    auto dog = std::make_shared<AnimatedDog>("MyPuppy");
    dog->setPosition(glm::vec3(0.5f, 0.05f, 3.0f));  // 设置一个初始位置
    dog->setScale(15.0f);                          // 设置一个合适的缩放
    dog->setFrameDuration(0.05f);                  // 设置动画速度 (20 FPS)
    // 4. 初始化小狗，加载所有模型帧
    dog->initialize(puppyFramePaths, dogModelBasePath);
    // 5. 将小狗添加到场景中
    objects_["MyPuppy"] = dog;
}

void SceneManager::update(float deltaTime) {
    for (auto& [name, object] : objects_) {
        object->update(deltaTime);
    }
}

// *** draw 函数的全新实现 ***
void SceneManager::draw(const Shader& defaultShader, const Shader& waterShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition, const AmbientLight& ambientLight, const DirectionalLight& directionalLight, const SpotLight& spotLight) {
    // 遍历场景中的所有对象
    for (auto const& [name, object] : objects_) {

        // 使用 dynamic_cast 在运行时检查这个对象是不是一个 PoolObject
        PoolObject* poolObject = dynamic_cast<PoolObject*>(object.get());

        if (poolObject) {
            // --- 如果是 PoolObject，调用它专属的、需要两个着色器的 draw 函数 ---
            poolObject->draw(defaultShader, waterShader, view, projection, cameraPosition,ambientLight, directionalLight, spotLight);
        } else {
            // --- 如果是其他普通对象（如长颈鹿、树），调用它们通用的、只需要一个着色器的 draw 函数 ---
            // 确保你的 SceneObject 基类有这个虚函数
            if(object) { // 简单的安全检查
                // 假设其他对象的 draw 函数只需要一个 shader
                // 注意：这里我们传递 defaultShader
                object->draw(defaultShader, view, projection,ambientLight,  directionalLight, spotLight);
            }
        }
    }
}