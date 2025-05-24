#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>

class Shader {
public:
    // 着色器 ID
    unsigned int id_;

    // 加载并编译着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    // 更换着色器
    void use() const;

    // 设置 uniform 变量
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;

};
#endif