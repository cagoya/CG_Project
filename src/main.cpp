#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ground.h"
#include "wall.h" 
#include "camera.h"
#include "roof.h"
#include "door.h"
#include "window.h"
#include "floor.h"
#include "table.h"
#include "shader.h"

// 主程序顶点着色器
const char* vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;

	out vec3 ourColor;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
	    gl_Position = projection * view * model * vec4(aPos, 1.0);
	    ourColor = aColor;
	}
)";

// 主程序片段着色器(R,G,B)
const char* fragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColor;

	in vec3 ourColor;

	void main()
	{
	    FragColor = vec4(ourColor, 1.0);
	}
)";

// 回调函数，因为它们需要一些主程序的信息，所以就直接放在主程序里面了
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Camera& camera, float deltaTime);

// 窗口宽度和高度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 摄像机对象
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

// 鼠标状态
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 时间管理
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

    // 锁定鼠标光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // 3. glad: 加载所有OpenGL函数指针
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // 4. 构建和编译着色器程序
    unsigned int shaderProgram = compileShaders(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0) {
        glfwTerminate();
        return -1;
    }


    // 5. 创建并设置场景中的物体对象
    Ground courtyardGround;
    courtyardGround.setup();

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

    Table houseTable;
    houseTable.setup();


    // 6. 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 7. 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, camera, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // 设置视图和投影矩阵
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // --- 绘制场景中的物体 ---

        // 绘制地面，后续可以考虑实现无限大效果，这里是有限大的高度为0的正方形
        glm::mat4 groundModelMatrix = glm::mat4(1.0f);
        courtyardGround.draw(shaderProgram, groundModelMatrix);

        // 计算房屋的模型矩阵 
        glm::mat4 houseModelMatrix = glm::mat4(1.0f);
        // 将 x 和 z 坐标放大 2 倍，y 坐标保持不变
        houseModelMatrix = glm::scale(houseModelMatrix, glm::vec3(2.0f, 1.0f, 2.0f));

        houseWall.draw(shaderProgram, houseModelMatrix);
        houseFloor.draw(shaderProgram, houseModelMatrix);
        houseRoof.draw(shaderProgram, houseModelMatrix);

        glm::mat4 doorModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.51f)); // 略微向外贴
        houseDoor.draw(shaderProgram, doorModelMatrix);

        glm::mat4 windowModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.51f, 0.0f, 0.0f)); // 略微向外贴
        houseWindow.draw(shaderProgram, windowModelMatrix);

        // 调整桌子的大小
        glm::mat4 tableModelMatrix = glm::mat4(1.0f);
        tableModelMatrix = glm::scale(tableModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        houseTable.draw(shaderProgram, tableModelMatrix);

        // --- 在这里添加其他对象的绘制 ---
        // --------------------------------

        // GLFW: 交换缓冲区和轮询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 8. 清理资源
    glDeleteProgram(shaderProgram);

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