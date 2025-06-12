#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "inside/inside.h"
#include "outside/outside.h"
#include "base/camera.h"
#include "base/shader.h"
#include "base/skybox.h"
#include "base/light.h"
#include "base/LightingPanel.h"
#include "ObjectModel/ObjectModel.h"
#include "base/SwimmingPool.h"  // 添加游泳池头文件
#include "base/SceneManager.h"
// 控制窗口
#include "base/ImGuiController.h"
#include  "base/ModelTransformPanel.h"
#include "base/characterPanel.h"

// 生成唯一文件名
#include <chrono>
#include <iomanip>
#include <sstream>

// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "base/AnimatedDog.h"
#include "base/PathHelper.h"
// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); // 增加了一个回调 目的：alt
void handleDogInput(GLFWwindow* window, float deltaTime);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

enum class CameraMode {
    FREE_ROAM,
    DOG_FOLLOW
};
CameraMode currentCameraMode = CameraMode::FREE_ROAM;
float mouseXOffset = 0.0f; // 用于在跟随模式下暂存鼠标X轴偏移

Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
bool isOrbitingCamera = false;             // 用于记录中键是否被按下
float cameraOrbitYaw = glm::radians(180.0f); // 相机环绕的水平角度
float cameraOrbitPitch = glm::radians(20.0f); // 相机环绕的俯仰角度
// 阴影贴图参数
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;

// 鼠标状态
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cursorLocked = true; //鼠标初始状态是锁定的

// 时间管理
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 导入模型的参数
static float modelScaleFactor = 0.003f;// 缩放
static glm::vec3 modelPosition = glm::vec3(4.0f, 0.2f, 2.0f);// 位置

//光线
AmbientLight ambientLight;
SpotLight spotLight;
DirectionalLight directionalLight;
Material material;

// 汉字
std::string sentence;
int font_choice = 0;
float font_size = 64.0f;
int R = 0;
int G = 0;
int B = 0;
const char* fonts[] = { "STSONG.TTF", "simfang.ttf", "simhei.ttf", "STKAITI.TTF", "STLITI.TTF" };
ImGuiController imguiController;
int main()
{
    // 1. GLFW: 初始化和配置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS 需要
	#endif

    // 2. GLFW: 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Courtyard with House and Roof", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // 锁定鼠标光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // 3. glad: 加载所有OpenGL函数指针
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    //创建深度帧缓冲和深度纹理
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 4. 构建和编译着色器程序
    Shader objModelShader(PathHelper::get("media/shader/objModel/objModel.vert.glsl").c_str(), PathHelper::get("media/shader/objModel/objModel.frag.glsl").c_str());
    Shader skyboxShader(PathHelper::get("media/shader/skybox/skybox.vert.glsl").c_str(), PathHelper::get("media/shader/skybox/skybox.frag.glsl").c_str());
    Shader mainShader(PathHelper::get("media/shader/main/main.vert.glsl").c_str(), PathHelper::get("media/shader/main/main.frag.glsl").c_str());
    Shader waterShader(PathHelper::get("media/shader/water/water.vert.glsl").c_str(), PathHelper::get("media/shader/water/water.frag.glsl").c_str());
    // 添加调试信息
    std::cout << "Checking shader uniforms for objModelShader:" << std::endl;
    GLint numUniforms;
    glGetProgramiv(objModelShader.id_, GL_ACTIVE_UNIFORMS, &numUniforms);
    std::cout << "Number of active uniforms: " << numUniforms << std::endl;

    for (GLint i = 0; i < numUniforms; i++) {
        char uniformName[256];
        GLint size;
        GLenum type;
        glGetActiveUniform(objModelShader.id_, i, sizeof(uniformName), NULL, &size, &type, uniformName);
        std::cout << "Uniform " << i << ": " << uniformName << " (Type: " << type << ", Size: " << size << ")" << std::endl;
    }
    Shader depthShader(PathHelper::get("media/shader/shadow/shadow.vert.glsl").c_str(), PathHelper::get("media/shader/shadow/shadow.frag.glsl").c_str());


    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    imguiController.Init(window);
    auto modelPanel = std::make_shared<ModelTransformPanel>("ModelController", modelPosition, modelScaleFactor, imguiController.GetIO());
    imguiController.AddPanel(modelPanel);
    auto lightPanel = std::make_shared<LightingPanel>("LightController", imguiController.GetIO(), spotLight, ambientLight, directionalLight, material);
    imguiController.AddPanel(lightPanel);
    auto characterPanel = std::make_shared<CharacterPanel>("CharacterController", imguiController.GetIO(), sentence, font_choice, font_size,R,G,B );
    imguiController.AddPanel(characterPanel);


    // 导入天空盒
    
    std::vector<std::string> faces = { "right.jpg", "left.jpg", "top.jpg", "down.jpg", "front.jpg", "back.jpg" };
    for (auto& it:faces)
    {
        it = "../../media/skybox/" + it;
    }
    Skybox sky(faces);

    Inside inside;
    inside.setup();

    Outside outside;
    outside.setup();

    // 6. 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 初始化场景管理器
    SceneManager::getInstance().initialize();


    // 设置光照参数
    ambientLight.color = glm::vec3(1.0f);
    ambientLight.intensity = 0.2f;

    directionalLight.direction = glm::vec3(0.4f, -1.0f, 0.5f);
    directionalLight.color = glm::vec3(1.0f);
    directionalLight.intensity = 0.5f;

    spotLight.position = camera.Position;
    spotLight.direction = camera.Front;
    spotLight.color = glm::vec3(1.0f);
    spotLight.intensity = 1.0f;
    spotLight.angle = glm::cos(glm::radians(12.5f));
    spotLight.kc = 1.0f;
    spotLight.kl = 0.09f;
    spotLight.kq = 0.032f;

    // 设置材质参数
    material.ns = 32.0f;

    // 7. 渲染循环
    while (!glfwWindowShouldClose(window))
    {
       // GLenum err = glGetError();
        //if (err != GL_NO_ERROR) std::cerr << "OpenGL Error: " << err << std::endl;
        glDisable(GL_CULL_FACE);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // 如果窗口最小化或隐藏，使用上次的有效尺寸
        static int lastWidth = 800, lastHeight = 600;
        if (width <= 0 || height <= 0) {
            width = lastWidth;
            height = lastHeight;
        } else {
            lastWidth = width;
            lastHeight = height;
        }

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //ImGui 开始新帧
        imguiController.NewFrame(); 
        imguiController.DrawUI();
        processInput(window, camera, deltaTime);
        handleDogInput(window, deltaTime);
        SceneManager::getInstance().update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 设置视图和投影矩阵
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);

        if (currentCameraMode == CameraMode::DOG_FOLLOW) {
            auto dog_ptr = SceneManager::getInstance().getObject("MyPuppy");
            if (auto dog = std::dynamic_pointer_cast<AnimatedDog>(dog_ptr)) {
                // 1. 定义相机与小狗的距离
                float distance = 4.0f;
                // 2. 定义相机看向的目标点（小狗的头部位置）
                glm::vec3 targetPos = dog->getPosition() + glm::vec3(0.0f, 0.5f, -1.0f);
                // 3. 根据环绕角度计算相机的位置偏移量
                float offsetX = distance * sin(cameraOrbitYaw) * cos(cameraOrbitPitch);
                float offsetY = distance * sin(cameraOrbitPitch);
                float offsetZ = distance * cos(cameraOrbitYaw) * cos(cameraOrbitPitch);

                // 4. 计算最终的相机位置
                camera.Position = targetPos + glm::vec3(offsetX, offsetY, offsetZ);
                // 5. 强制相机看向目标点，并使用世界空间的垂直向上向量来避免倾斜
                view = glm::lookAt(camera.Position, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));
            }
        } else {
            view = camera.GetViewMatrix();
        }
        // --- 绘制阴影贴图 ---
        // 用光源视角渲染深度贴图

        float near_plane = 1.0f, far_plane = 20.0f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(
            directionalLight.direction * -10.0f, // 光源位置
            glm::vec3(0.0f),                     // 看向原点
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        glm::mat4 model = glm::mat4(1.0f);
        depthShader.use();
        depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);


        inside.draw(depthShader, model, sentence, fonts[font_choice], font_size, R, G, B,false);
        outside.draw(depthShader, glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)),false);
        SceneManager::getInstance().draw(depthShader, waterShader, view, projection, camera.Position,ambientLight, directionalLight, spotLight);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height); // 恢复视口

        // 绘制天空盒
        skyboxShader.use();
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        sky.draw(skyboxShader, view, projection);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        // --- 绘制场景中的物体 ---
        glm::mat4 houseModel = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));


        mainShader.use();
        mainShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        mainShader.setInt("shadowMap", 1);
        spotLight.direction = camera.Front;
        spotLight.position = camera.Position;
        // 这些是共用的，材质是每个物品的属性
        mainShader.setMat4("view", view);
        mainShader.setMat4("projection", projection);
        mainShader.setVec3("cameraPosition", camera.Position);
        mainShader.setFloat("material.ns", material.ns);
        mainShader.setVec3("ambientLight.color", ambientLight.color);
        mainShader.setFloat("ambientLight.intensity", ambientLight.intensity);
        mainShader.setVec3("directionalLight.direction", directionalLight.direction);
        mainShader.setVec3("directionalLight.color", directionalLight.color);
        mainShader.setFloat("directionalLight.intensity", directionalLight.intensity);
        mainShader.setVec3("spotLight.position", spotLight.position);
        mainShader.setVec3("spotLight.direction", spotLight.direction);
        mainShader.setVec3("spotLight.color", spotLight.color);
        mainShader.setFloat("spotLight.intensity", spotLight.intensity);
        mainShader.setFloat("spotLight.angle", spotLight.angle);
        mainShader.setFloat("spotLight.kc", spotLight.kc);
        mainShader.setFloat("spotLight.kl", spotLight.kl);
        mainShader.setFloat("spotLight.kq", spotLight.kq);

        inside.draw(mainShader, model, sentence, fonts[font_choice], font_size, R, G, B,true);
        outside.draw(mainShader, glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)),true);


        // --- 在这里添加其他对象的绘制 ---
        objModelShader.use(); // 确保在使用着色器之前先激活它
        
        // 为新的 objModelShaderProgram 设置 view 和 projection 矩阵
        objModelShader.setMat4("view", view);
        objModelShader.setMat4("projection", projection);

        // 添加相机位置
        objModelShader.setVec3("viewPos", camera.Position);

        // 添加时间uniform
        float currentTime = static_cast<float>(glfwGetTime());
        objModelShader.setFloat("time", currentTime);

        // 设置材质
        objModelShader.setInt("material.diffuse", 0);
        objModelShader.setInt("material.normal", 1);

        // 设置光照参数
        objModelShader.setVec3("ambientColor", ambientLight.color);
        objModelShader.setFloat("ambientIntensity", ambientLight.intensity);

        // 方向光
        objModelShader.setVec3("directionalLight.direction", directionalLight.direction);
        objModelShader.setVec3("directionalLight.color", directionalLight.color);
        objModelShader.setFloat("directionalLight.intensity", directionalLight.intensity);

        // 聚光灯
        objModelShader.setVec3("spotLight.position", spotLight.position);
        objModelShader.setVec3("spotLight.direction", spotLight.direction);
        objModelShader.setVec3("spotLight.color", spotLight.color);
        objModelShader.setFloat("spotLight.intensity", spotLight.intensity);
        objModelShader.setFloat("spotLight.angle", spotLight.angle);
        objModelShader.setFloat("spotLight.kc", spotLight.kc);
        objModelShader.setFloat("spotLight.kl", spotLight.kl);
        objModelShader.setFloat("spotLight.kq", spotLight.kq);

        // 添加调试输出
        static float lastDebugTime = 0.0f;
        if (currentTime - lastDebugTime > 1.0f) {  // 每秒输出一次
            std::cout << "Light values:" << std::endl;
            std::cout << "Ambient: " << ambientLight.intensity << std::endl;
            std::cout << "Directional: " << directionalLight.intensity << std::endl;
            std::cout << "Spot: " << spotLight.intensity << std::endl;
            lastDebugTime = currentTime;
        }

        GLint diffuseTexSamplerLoc = glGetUniformLocation(objModelShader.id_, "texture_diffuse1");
        if (diffuseTexSamplerLoc != -1) {
            glUniform1i(diffuseTexSamplerLoc, 0);
        }
        else {
            std::cout << "WARNING: Sampler uniform 'texture_diffuse1' not found in objModelShader!" << std::endl;
        }


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_BLEND);
        // 使用objModel着色器渲染树
        objModelShader.use();
        objModelShader.setMat4("view", view);
        objModelShader.setMat4("projection", projection);
        objModelShader.setVec3("cameraPosition", camera.Position);

        // 设置材质参数
        objModelShader.setFloat("material.ns", material.ns);
        
        // 设置光照参数
        objModelShader.setVec3("ambientLight.color", ambientLight.color);
        objModelShader.setFloat("ambientLight.intensity", ambientLight.intensity);
        
        objModelShader.setVec3("directionalLight.direction", directionalLight.direction);
        objModelShader.setVec3("directionalLight.color", directionalLight.color);
        objModelShader.setFloat("directionalLight.intensity", directionalLight.intensity);
        
        objModelShader.setVec3("spotLight.position", spotLight.position);
        objModelShader.setVec3("spotLight.direction", spotLight.direction);
        objModelShader.setVec3("spotLight.color", spotLight.color);
        objModelShader.setFloat("spotLight.intensity", spotLight.intensity);
        objModelShader.setFloat("spotLight.angle", spotLight.angle);
        objModelShader.setFloat("spotLight.kc", spotLight.kc);
        objModelShader.setFloat("spotLight.kl", spotLight.kl);
        objModelShader.setFloat("spotLight.kq", spotLight.kq);

        // 渲染场景中的所有对象(包括树)
        // SceneManager::getInstance().draw(objModelShader, view, projection);
        SceneManager::getInstance().draw(objModelShader, waterShader, view, projection, camera.Position,ambientLight, directionalLight, spotLight);
        // --------------------------------
        // ImGui 渲染绘制的面板数据
        imguiController.Render();

        // GLFW: 交换缓冲区和轮询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 8. 清理资源
    glDeleteProgram(skyboxShader.id_);

    //清理本模型资源
    glDeleteProgram(objModelShader.id_);

    //Gui 清理
    imguiController.Shutdown();

    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 只有在自由漫游模式下，按键才控制相机
    if (currentCameraMode == CameraMode::FREE_ROAM) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (!cursorLocked) { firstMouse = true; return; }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (currentCameraMode == CameraMode::DOG_FOLLOW) {
        if (isOrbitingCamera) {
            const float orbitSensitivity = 0.008f;
            cameraOrbitYaw -= xoffset * orbitSensitivity;
            cameraOrbitPitch -= yoffset * orbitSensitivity;
            cameraOrbitPitch = glm::clamp(cameraOrbitPitch, glm::radians(-40.0f), glm::radians(80.0f));
        } else {
            mouseXOffset += xoffset;
        }
    } else {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // 只关心鼠标中键
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            isOrbitingCamera = true; // 当中键被按下，设置标志为true
        } else if (action == GLFW_RELEASE) {
            isOrbitingCamera = false; // 当中键被抬起，设置标志为false
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
        cursorLocked = !cursorLocked;
        glfwSetInputMode(window, GLFW_CURSOR, cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        if (currentCameraMode == CameraMode::FREE_ROAM) {
            currentCameraMode = CameraMode::DOG_FOLLOW;
            camera.Zoom = 30.0f;
            // 当切换到跟随模式时，将当前的相机朝向转换为初始的环绕角度
            auto dog_ptr = SceneManager::getInstance().getObject("MyPuppy");
            if (auto dog = std::dynamic_pointer_cast<AnimatedDog>(dog_ptr)) {
                glm::vec3 dir = glm::normalize(dog->getPosition() - camera.Position);
                cameraOrbitYaw = atan2(dir.x, dir.z);
                cameraOrbitPitch = asin(dir.y);
            }
        } else {
            currentCameraMode = CameraMode::FREE_ROAM;
            camera.Zoom = 45.0f;
        }
    }
}

void handleDogInput(GLFWwindow* window, float deltaTime) {
    auto dog_ptr = SceneManager::getInstance().getObject("MyPuppy");
    auto dog = std::dynamic_pointer_cast<AnimatedDog>(dog_ptr);
    if (!dog) return;
    // 只有在跟随模式下，输入才控制小狗
    if (currentCameraMode == CameraMode::DOG_FOLLOW) {
        // --- 鼠标控制转向 ---
        if (!isOrbitingCamera) {
            const float mouseSensitivity = 0.008f;
            dog->rotationY -= mouseXOffset * mouseSensitivity;
        }
        mouseXOffset = 0.0f;
        // --- 键盘输入状态获取 ---
        bool key_w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        bool key_a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        bool key_s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        bool key_d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
        bool key_shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
        bool isTryingToMove = key_w || key_a || key_s || key_d;

        // --- 移动计算 ---
        float currentMoveSpeed = (key_w && key_shift) ? dog->runSpeed : dog->walkSpeed;
        glm::vec3 moveDirection(0.0f);

        // 只有在有移动意图时才计算方向，避免不必要的计算
        if (isTryingToMove) {
            glm::vec3 forward(sin(dog->rotationY), 0.0f, cos(dog->rotationY));
            glm::vec3 right(forward.z, 0.0f, -forward.x);
            if (key_w) moveDirection += forward;
            if (key_s) moveDirection -= forward;
            if (key_a) moveDirection -= right;
            if (key_d) moveDirection += right;
        }

        if(glm::length(moveDirection) > 0.0001f) // 用一个很小的数而不是0.0来避免浮点数精度问题
        {
             // 只有在向量长度不为0时，才进行归一化和移动
             dog->setPosition(dog->getPosition() + glm::normalize(moveDirection) * currentMoveSpeed * deltaTime);
             // 让小狗的身体总是朝向它移动的方向 (仅当不使用鼠标转向时)
             if(isOrbitingCamera){ // 如果在环绕相机，让身体朝向移动方向
                 dog->rotationY = atan2(moveDirection.x, moveDirection.z);
             }
        }
        // --- 跳跃逻辑 ---
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            dog->jump();
        }
        // --- 动画状态机 ---
        if (dog->isOnGround()) {
            if (key_shift && !isTryingToMove) {
                dog->setState(DogState::SIT);
            } else if (key_w && key_shift) {
                dog->setState(DogState::RUN);
            } else if (isTryingToMove) {
                dog->setState(DogState::WALK);
            } else {
                if (dog->getCurrentState() != DogState::SIT) {
                     dog->setState(DogState::IDLE);
                }
            }
        }
    }
}
void scroll_callback(GLFWwindow* window, double xoffsetIn, double yoffsetIn)
{
    float yoffset = static_cast<float>(yoffsetIn);
    camera.ProcessMouseScroll(yoffset);
}
void takeScreenshot(GLFWwindow* window, const std::string& filename) {
    int width, height;
    // 获取帧缓冲区的大小，而不是窗口大小，这对于高DPI显示器很重要
    glfwGetFramebufferSize(window, &width, &height);
    // 分配内存来存储像素数据 (R, G, B 三个通道)
    GLubyte* pixels = new GLubyte[3 * width * height];
    // 设置像素打包对齐方式为1字节，防止因行末填充导致图像错乱
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    // 使用 stb_image_write 将像素数据保存为 PNG 文件
    // 参数: 文件名, 宽, 高, 通道数(RGB是3), 像素数据, 每行的字节数
    if (stbi_write_png(filename.c_str(), width, height, 3, pixels, width * 3)) {
        std::cout << "Screenshot saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to save screenshot." << std::endl;
    }
    delete[] pixels;
}
