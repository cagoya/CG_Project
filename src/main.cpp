#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "wall.h"
#include "ground.h"
#include "camera.h"
#include "roof.h"
#include "door.h"
#include "window.h"
#include "floor.h"
#include "table.h"
#include "shader.h"
#include "skybox.h"
#include "ObjectModel/ObjectModel.h"

// 控制窗口
#include "UIsystem/ImGuiController.h"
#include "UIsystem/ModelTransformPanel.h"

// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); // 增加了一个回调 目的：alt

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

// 鼠标状态
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cursorLocked = true; //鼠标初始状态是锁定的

// 时间管理
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 导入模型的参数
static float modelScaleFactor = 1.0f;// 缩放
static glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);// 位置

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

    // 4. 构建和编译着色器程序
    Shader mainShader = Shader("../../media/shader/main/main_vs.txt", "../../media/shader/main/main_fs.txt");
    //Shader objModelShader = Shader("../../media/shader/objModel/objModelShader_vs.txt","../../media/shader/objModel/objModelShader_fs.txt");
    Shader skyboxShader("../../media/shader/skybox/skybox_vs.txt", "../../media/shader/skybox/skybox_fs.txt");

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    //imguiController.Init(window); // 初始化 ImGui
    // 创建并注册UI一个面板 
    //auto modelPanel = std::make_shared<ModelTransformPanel>("ModelController", modelPosition, modelScaleFactor, imguiController.GetIO());
    //imguiController.AddPanel(modelPanel);
    // 可以多做几个面板！

    // 5. 创建并设置场景中的物体对象
    /*
    ObjectModel myModel;
    std::string objRelativePath = "../../media/Minotaur_Female_Lores.obj";//只需输入.obj位置即可
    std::string mtlBaseRelativePath = ""; // 斜杠害人不浅 如果有mtl 和.obj放在一起即可（如果在一个folder里面，空字符串也可以） 填对应文件夹位置就行 是给texture提供相关支持的

    // 尝试加载模型
    if (!myModel.load(objRelativePath, mtlBaseRelativePath)) {
        std::cerr << "Failed to load OBJ model using relative paths!" << std::endl;
        std::cerr << "Attempted OBJ path: " << objRelativePath << std::endl;
        std::cerr << "Attempted MTL base path: " << mtlBaseRelativePath << std::endl;
    }
    else {
        std::cout << "Successfully loaded OBJ model using relative paths!" << std::endl;
    }*/

    // 导入天空盒
    
    std::vector<std::string> faces = { "right.jpg", "left.jpg", "top.jpg", "down.jpg", "front.jpg", "back.jpg" };
    for (auto& it:faces)
    {
        it = "../../media/skybox/bak1/" + it;
    }
    Skybox sky(faces);

    Ground ground;
    ground.setup();

    Wall houseWall;
    houseWall.setup();

    GableRoof houseRoof;
    houseRoof.setup();

    Door houseDoor;
    houseDoor.setup();

    Window houseWindow;
    houseWindow.setup();

    Floor houseFloor;
    houseFloor.setup();

    Table table;
    table.setup();

    // 6. 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 7. 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //ImGui 开始新帧
        //imguiController.NewFrame(); 
        //imguiController.DrawUI();

        processInput(window, camera, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 设置视图和投影矩阵
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // 绘制天空盒
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        sky.draw(skyboxShader, view, projection);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        // --- 绘制场景中的物体 ---
        mainShader.use();
        mainShader.setMat4("view", view);
        mainShader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);

        ground.draw(mainShader, model);
        houseWall.draw(mainShader, model);
        houseFloor.draw(mainShader, model);
        houseRoof.draw(mainShader, model);
        houseDoor.draw(mainShader, model);
        houseWindow.draw(mainShader, model);

        
        

        // --- 在这里添加其他对象的绘制 ---
         
        /*objModelShader.use(); // 换到新的着色器程序
        
        // 为新的 objModelShaderProgram 设置 view 和 projection 矩阵
        // (通常和上面的 view, projection 矩阵是一样的，但需要为当前激活的着色器重新设置)
        glUniformMatrix4fv(glGetUniformLocation(objModelShader.id_, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(objModelShader.id_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // myModel 的模型矩阵
        glm::mat4 modelForMyObj = glm::mat4(1.0f); // 单位矩阵
        modelForMyObj = glm::translate(modelForMyObj, modelPosition); // 调整模型在场景中的位置
        modelForMyObj = glm::scale(modelForMyObj, glm::vec3(modelScaleFactor));   // 调整模型的大小
        // 实现的ObjectModel::draw 方法内部会设置自己的 model 矩阵 uniform 和绑定 VAO
        myModel.draw(objModelShader.id_, modelForMyObj);*/

        // --------------------------------
        // ImGui 渲染绘制的面板数据
        imguiController.Render();

        // GLFW: 交换缓冲区和轮询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 8. 清理资源
    glDeleteProgram(mainShader.id_);
    glDeleteProgram(skyboxShader.id_);

    //清理本模型资源
    //glDeleteProgram(objModelShader.id_);

    //Gui 清理
    imguiController.Shutdown();

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