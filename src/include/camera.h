#ifndef CAMERA_H
#define CAMERA_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ������ܵ�������ƶ�����
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// �����Ĭ��ֵ������һ������ŷ���ǵ����
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // ���������
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // ŷ����
    float Yaw;
    float Pitch;

    // �����ѡ��
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // ���캯�� (����ָ����ʼλ�ã�����ʹ��Ĭ��λ��)
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // ��ȡ��ͼ����
    glm::mat4 GetViewMatrix() const;

    // ���ݼ������봦���ƶ�
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // ��������ƶ������ӽǱ仯
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // ���������ִ�����Ұ�仯 (����) - Ӱ��ͶӰ����
    void ProcessMouseScroll(float yoffset);

private:
    // ����ŷ���Ǹ���������� Front, Right �� Up ����
    void updateCameraVectors();
};

#endif // CAMERA_H