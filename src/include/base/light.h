#ifndef LIGHT_H
#define LIGHT_H
#include <vector>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

class Light {
public:
	glm::vec3 color = { 1.0f,1.0f,1.0f };    // 光源颜色
	float intensity = 1.0f;    // 光源强度
};

class AmbientLight : public Light {
};

class DirectionalLight : public Light {
public:
	glm::vec3 direction = { 0.0f,0.0f,-1.0f };
};
class SpotLight : public Light {
public:
	glm::vec3 position = { 0.0f,2.0f,0.0f };
	glm::vec3 direction = { 0.0f,0.0f,-1.0f }; // 光源方向
	float angle = glm::radians(60.0f);         // 光源角度
	float kc = 1.0f;
	float kl = 0.0f;
	float kq = 1.0f;
};
class PointLight : public Light {
public:
	glm::vec3 position = { 2.0f,0.0f,0.0f }; // 光源位置
	float kc = 1.0f;
	float kl = 0.0f;
	float kq = 1.0f;
};
class Material {
public:
	glm::vec3 ka = glm::vec3(0.3f, 0.3f, 0.3f);
	glm::vec3 kd = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ks = glm::vec3(1.0f, 1.0f, 1.0f);
	float ns = 10.0f;
};
#endif