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

// 控制窗口
#include "base/ImGuiController.h"
#include  "base/ModelTransformPanel.h"
#include "base/characterPanel.h"

// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); // 增加了一个回调 目的：alt

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

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

// 准备实现模型拖动
bool isInEditMode = false;
ObjectModel* selectedModel = nullptr; // 指向当前选中的 ObjectModel
bool isDragging = false;              // 是否正在拖动模型
glm::vec3 dragInitialIntersectPoint;  // 拖动开始时射线与拖动平面的交点 (世界空间)
glm::vec3 dragInitialModelPosition;

// 汉字
std::string sentence;
int font_choice = 0;
float font_size = 64.0f;
int R = 0;
int G = 0;
int B = 0;
const char* fonts[] = { "STSONG.TTF", "simfang.ttf", "simhei.ttf", "STKAITI.TTF", "STLITI.TTF" };

// 修改游泳池的初始参数
static float poolScale = 1.0f;  // 增大初始缩放
static glm::vec3 poolPosition = glm::vec3(5.5f, 0.4f, 2.0f);  // 将游泳池放在场景中心

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
    Shader objModelShader = Shader("../../media/shader/objModel/objModel.vert.glsl","../../media/shader/objModel/objModel.frag.glsl");
    Shader skyboxShader("../../media/shader/skybox/skybox.vert.glsl", "../../media/shader/skybox/skybox.frag.glsl");
    Shader mainShader = Shader("../../media/shader/main/main.vert.glsl", "../../media/shader/main/main.frag.glsl");

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

    //Shader waterShader = Shader("", "../../media/shader/water/water_fs.glsl");
    Shader depthShader("../../media/shader/shadow/shadow.vert.glsl", "../../media/shader/shadow/shadow.frag.glsl");

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    imguiController.Init(window);
    auto modelPanel = std::make_shared<ModelTransformPanel>("ModelController", modelPosition, modelScaleFactor, imguiController.GetIO());
    imguiController.AddPanel(modelPanel);
    auto lightPanel = std::make_shared<LightingPanel>("LightController", imguiController.GetIO(), spotLight, ambientLight, directionalLight, material);
    imguiController.AddPanel(lightPanel);
    auto characterPanel = std::make_shared<CharacterPanel>("CharacterController", imguiController.GetIO(), sentence, font_choice, font_size,R,G,B );
    imguiController.AddPanel(characterPanel);

    // 5. 创建并设置场景中的物体对象
    SwimmingPool swimmingPool;
    if (!swimmingPool.initialize()) {
        std::cerr << "Failed to initialize swimming pool!" << std::endl;
    }
    swimmingPool.setPosition(poolPosition);
    swimmingPool.setScale(poolScale);

    // 添加调试信息
    std::cout << "Swimming pool initialized with:" << std::endl;
    std::cout << "Position: (" << poolPosition.x << ", " << poolPosition.y << ", " << poolPosition.z << ")" << std::endl;
    std::cout << "Scale: " << poolScale << std::endl;

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

        // 修改游泳池控制面板的范围
        ImGui::Begin("Swimming Pool Control");
        if (ImGui::SliderFloat3("Position", &poolPosition.x, -20.0f, 20.0f)) {
            swimmingPool.setPosition(poolPosition);
        }
        if (ImGui::SliderFloat("Scale", &poolScale, 0.001f, 0.005f)) {
            swimmingPool.setScale(poolScale);
        }
        ImGui::End();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 设置视图和投影矩阵
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        //std::cout << height << " " << width << std::endl;
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
        //glDisable(GL_CULL_FACE);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);


        inside.drawShadow(depthShader, model, sentence, fonts[font_choice], font_size, R, G, B,false);
        outside.drawShadow(depthShader, glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)),false);

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
        glm::mat4 model = glm::mat4(1.0f);
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

        // 绘制游泳池
        swimmingPool.update(deltaTime);
        swimmingPool.draw(objModelShader.id_, glm::translate(glm::mat4(1.0f), poolPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(poolScale)));

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

void processInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if(!cursorLocked){
        return;
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffsetIn, double yoffsetIn)
{
    float yoffset = static_cast<float>(yoffsetIn);
    camera.ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // ESC退出功能
        glfwSetWindowShouldClose(window, true);

    // 关注左Alt键
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
    {
        cursorLocked = !cursorLocked; // 切换锁定状态

        if (cursorLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true; // 重置firstMouse，防止重新锁定时相机跳动
            std::cout << "Cursor locked." << std::endl;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            std::cout << "Cursor unlocked." << std::endl;
        }
    }
}