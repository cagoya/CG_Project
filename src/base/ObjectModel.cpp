#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include "ObjectModel.h" // 假设 ObjectModel.h 定义了 Vertex 结构和类成员
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <map>
#include <glm/gtc/type_ptr.hpp> 

// 辅助函数：分割字符串
static inline std::vector<std::string> objSplitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) { // 确保不添加空token
            tokens.push_back(token);
        }
    }
    return tokens;
}

// 辅助函数：解析面顶点字符串 "v/vt/vn"
// 返回 false 如果基础顶点索引 v_idx 解析失败或无效
static inline bool objParseFaceVertexIndices(const std::string& faceVertexStr,
    int& v_idx, int& vt_idx, int& vn_idx,
    int max_v_idx, int max_vt_idx, int max_vn_idx) {
    v_idx = vt_idx = vn_idx = -1;

    std::vector<std::string> parts = objSplitString(faceVertexStr, '/');

    if (parts.empty() || parts[0].empty()) return false; // 顶点位置索引必须存在

    try {
        v_idx = std::stoi(parts[0]) - 1;
        if (v_idx < 0 || v_idx >= max_v_idx) return false; // 索引越界

        if (parts.size() > 1 && !parts[1].empty()) {
            vt_idx = std::stoi(parts[1]) - 1;
            if (vt_idx < 0 || vt_idx >= max_vt_idx) vt_idx = -1; // 无效纹理索引视为不存在
        }
        if (parts.size() > 2 && !parts[2].empty()) {
            vn_idx = std::stoi(parts[2]) - 1;
            if (vn_idx < 0 || vn_idx >= max_vn_idx) vn_idx = -1; // 无效法线索引视为不存在
        }
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "OBJECTMODEL WARNING: Malformed index in face component: " << faceVertexStr << " (" << ia.what() << ")" << std::endl;
        return false;
    }
    catch (const std::out_of_range& oor) {
        std::cerr << "OBJECTMODEL WARNING: Index out of range in face component: " << faceVertexStr << " (" << oor.what() << ")" << std::endl;
        return false;
    }
    return true;
}

ObjectModel::ObjectModel() : VAO(0), VBO(0), EBO(0) {}

ObjectModel::~ObjectModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    for (GLuint textureID : textureIDs) {
        glDeleteTextures(1, &textureID);
    }
}

bool ObjectModel::load(const std::string& path, const std::string& mtlBasePath) {
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;

    vertices.clear();
    indices.clear();
    for (GLuint textureID : textureIDs) {
        glDeleteTextures(1, &textureID);
    }
    textureIDs.clear();

    std::map<std::tuple<int, int, int>, unsigned int> vertexCache;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "OBJECTMODEL ERROR: Cannot open file: " << path << std::endl;
        return false;
    }

    std::string currentMaterialName = "";
    std::string materialLibraryFile = "";
    std::string currentObjectName = "default_object"; // OBJ 文件可以不指定对象名

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        // 移除可能的回车符，处理行尾空格
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        while (!line.empty() && isspace(line.back())) {
            line.pop_back();
        }
        while (!line.empty() && isspace(line.front())) {
            line.erase(0, 1);
        }


        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword.empty() || keyword[0] == '#') {
            continue;
        }
        else if (keyword == "v") {
            glm::vec3 pos;
            if (!(ss >> pos.x >> pos.y >> pos.z)) {
                std::cerr << "OBJECTMODEL WARNING: Malformed vertex data at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            temp_positions.push_back(pos);
        }
        else if (keyword == "vt") {
            glm::vec2 uv;
            if (!(ss >> uv.x >> uv.y)) { // 检查是否成功读取两个分量
                std::cerr << "OBJECTMODEL WARNING: Malformed texture coordinate data at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            // float w_coord; // 可选的 w
            // if (ss >> w_coord) { /* 处理w */ }
            temp_texCoords.push_back(uv);
        }
        else if (keyword == "vn") {
            glm::vec3 norm;
            if (!(ss >> norm.x >> norm.y >> norm.z)) {
                std::cerr << "OBJECTMODEL WARNING: Malformed vertex normal data at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            temp_normals.push_back(norm);
        }
        else if (keyword == "f") {
            std::vector<std::string> faceVertexStrings;
            std::string faceVertexToken;
            while (ss >> faceVertexToken) {
                faceVertexStrings.push_back(faceVertexToken);
            }

            if (faceVertexStrings.size() < 3) {
                // std::cerr << "OBJECTMODEL WARNING: Face with less than 3 vertices at line " << lineNumber << std::endl;
                continue;
            }

            int v_idx0, vt_idx0, vn_idx0;
            if (!objParseFaceVertexIndices(faceVertexStrings[0], v_idx0, vt_idx0, vn_idx0,
                temp_positions.size(), temp_texCoords.size(), temp_normals.size())) {
                std::cerr << "OBJECTMODEL WARNING: Invalid face vertex data at line " << lineNumber << " for component " << faceVertexStrings[0] << std::endl;
                continue; // 跳过整个面
            }


            for (size_t i = 1; i < faceVertexStrings.size() - 1; ++i) {
                int v_idx1, vt_idx1, vn_idx1;
                int v_idx2, vt_idx2, vn_idx2;

                bool v1_ok = objParseFaceVertexIndices(faceVertexStrings[i], v_idx1, vt_idx1, vn_idx1,
                    temp_positions.size(), temp_texCoords.size(), temp_normals.size());
                bool v2_ok = objParseFaceVertexIndices(faceVertexStrings[i + 1], v_idx2, vt_idx2, vn_idx2,
                    temp_positions.size(), temp_texCoords.size(), temp_normals.size());

                if (!v1_ok || !v2_ok) {
                    std::cerr << "OBJECTMODEL WARNING: Invalid face vertex data at line " << lineNumber
                        << " for components " << faceVertexStrings[i] << " or " << faceVertexStrings[i + 1] << std::endl;
                    break; // 跳过这个面剩余的三角形
                }


                std::tuple<int, int, int> face_indices_tuples[3] = {
                    std::make_tuple(v_idx0, vt_idx0, vn_idx0),
                    std::make_tuple(v_idx1, vt_idx1, vn_idx1),
                    std::make_tuple(v_idx2, vt_idx2, vn_idx2)
                };

                for (int j = 0; j < 3; ++j) {
                    std::tuple<int, int, int> current_key = face_indices_tuples[j];

                    if (vertexCache.find(current_key) == vertexCache.end()) {
                        Vertex new_vert;
                        int p_idx = std::get<0>(current_key); // 此时p_idx已知是有效的

                        new_vert.Position = temp_positions[p_idx];

                        int t_idx = std::get<1>(current_key);
                        if (t_idx != -1) {
                            new_vert.TexCoords = temp_texCoords[t_idx];
                            new_vert.TexCoords.y = 1.0f - new_vert.TexCoords.y;
                        }
                        else {
                            new_vert.TexCoords = glm::vec2(0.0f, 0.0f);
                        }

                        int n_idx = std::get<2>(current_key);
                        if (n_idx != -1) {
                            new_vert.Normal = temp_normals[n_idx];
                        }
                        else {
                            new_vert.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
                        }

                        vertices.push_back(new_vert);
                        unsigned int new_final_idx = static_cast<unsigned int>(vertices.size() - 1);
                        vertexCache[current_key] = new_final_idx;
                        indices.push_back(new_final_idx);
                    }
                    else {
                        indices.push_back(vertexCache[current_key]);
                    }
                }
            }
        }
        else if (keyword == "mtllib") {
            if (!(ss >> materialLibraryFile)) {
                std::cerr << "OBJECTMODEL WARNING: Malformed mtllib directive at line " << lineNumber << std::endl;
            }
        }
        else if (keyword == "usemtl") {
            if (!(ss >> currentMaterialName)) {
                std::cerr << "OBJECTMODEL WARNING: Malformed usemtl directive at line " << lineNumber << std::endl;
            }
            // 注意：ObjectModel 当前结构只支持一个整体的纹理列表 (textureIDs)
            // 真实的 usemtl 切换需要将模型分割成不同的 Mesh，每个 Mesh 关联自己的材质
            // 这里我们只记录最后或第一个遇到的 usemtl，并尝试加载其纹理 (如果 textureIDs 为空)
            if (!mtlBasePath.empty() && !currentMaterialName.empty() && textureIDs.empty()) {
                // 实际应用中，这里应该触发 .mtl 文件的解析，
                // 根据 currentMaterialName 查找 map_Kd (漫反射纹理)
                // 这里仅作打印，提示需要进一步工作
                std::cout << "ObjectModel: Encountered usemtl '" << currentMaterialName
                    << "'. (mtllib: '" << materialLibraryFile << "'). "
                    << "MTL parsing and specific texture loading for this material is not yet implemented in this basic loader."
                    << std::endl;

                // 示例：如果你想硬编码或基于约定加载一个纹理：
                // (你需要有实际的纹理文件，并且知道它的名字)
                // std::string exampleTextureName = currentMaterialName + "_diffuse.png"; // 假设的命名约定
                // std::string texturePath = mtlBasePath + exampleTextureName;
                // GLuint texID = loadTexture(texturePath.c_str());
                // if (texID != 0) {
                //     textureIDs.push_back(texID);
                // } else {
                //     std::cout << "ObjectModel: Could not load example texture: " << texturePath << std::endl;
                // }
            }
        }
        else if (keyword == "o" || keyword == "g") {
            // 对象名或组名。对于当前的 ObjectModel (单一网格结构)，
            // 我们可以记录第一个对象名，或者忽略这些指令。
            if (currentObjectName == "default_object") { // 只记录第一个对象名
                ss >> currentObjectName;
            }
        }
        // 's' (smoothing group) 等其他指令被忽略
    }
    file.close();

    if (vertices.empty()) {
        std::cerr << "OBJECTMODEL ERROR: No vertices were loaded or generated for " << path << std::endl;
        return false;
    }

    std::cout << "ObjectModel: Successfully parsed OBJ geometry from \"" << path << "\"." << std::endl;
    std::cout << "  (Object Name Hint: " << currentObjectName << ", Last Material Hint: " << currentMaterialName << ")" << std::endl;
    std::cout << "  Total unique vertices generated: " << vertices.size() << ", Total indices: " << indices.size() << std::endl;

    setupMesh();
    return true;
}
void ObjectModel::setupMesh() {

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);



    glBindVertexArray(VAO);



    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);



    // 位置

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // 顶点法线

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // 顶点纹理坐标

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));



    glBindVertexArray(0);

}



void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // 绑定漫反射纹理 (假设只有一个纹理且是漫反射)
    // 可能需要更复杂的逻辑来处理多种纹理或没有纹理的情况
    if (!textureIDs.empty()) {
        glActiveTexture(GL_TEXTURE0); // 激活纹理单元0
        glBindTexture(GL_TEXTURE_2D, textureIDs[0]); // 绑定此模型的纹理
        // 确保着色器中的 sampler2D uniform 设置为0
        // glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse1"), 0); // 通常在初始化时设置一次
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    if (!textureIDs.empty()) {
        glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理
    }
}



GLuint ObjectModel::loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unknown number of texture components: " << nrComponents << " for " << path << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data); // data 此时为nullptr，但调用也无害
        glDeleteTextures(1, &textureID);
        return 0;
    }
    return textureID;
}