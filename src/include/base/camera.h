#ifndef CAMERA_H
#define CAMERA_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 定义可能的摄像机移动方向
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// 摄像机默认值，这是一个基于欧拉角的相机
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // 摄像机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // 欧拉角
    float Yaw;
    float Pitch;

    // 摄像机选项
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // 构造函数 (可以指定初始位置，或者使用默认位置)
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // 获取视图矩阵
    glm::mat4 GetViewMatrix() const;

    // 根据键盘输入处理移动
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // 根据鼠标移动处理视角变化
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // 根据鼠标滚轮处理视野变化 (缩放) - 影响投影矩阵
    void ProcessMouseScroll(float yoffset);

private:
    // 根据欧拉角更新摄像机的 Front, Right 和 Up 向量
    void updateCameraVectors();
};

#endif // CAMERA_H