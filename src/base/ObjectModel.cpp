#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include "ObjectModel.h" // ���� ObjectModel.h ������ Vertex �ṹ�����Ա
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <map>
#include <glm/gtc/type_ptr.hpp> 

// �����������ָ��ַ���
static inline std::vector<std::string> objSplitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) { // ȷ������ӿ�token
            tokens.push_back(token);
        }
    }
    return tokens;
}

// ���������������涥���ַ��� "v/vt/vn"
// ���� false ��������������� v_idx ����ʧ�ܻ���Ч
static inline bool objParseFaceVertexIndices(const std::string& faceVertexStr,
    int& v_idx, int& vt_idx, int& vn_idx,
    int max_v_idx, int max_vt_idx, int max_vn_idx) {
    v_idx = vt_idx = vn_idx = -1;

    std::vector<std::string> parts = objSplitString(faceVertexStr, '/');

    if (parts.empty() || parts[0].empty()) return false; // ����λ�������������

    try {
        v_idx = std::stoi(parts[0]) - 1;
        if (v_idx < 0 || v_idx >= max_v_idx) return false; // ����Խ��

        if (parts.size() > 1 && !parts[1].empty()) {
            vt_idx = std::stoi(parts[1]) - 1;
            if (vt_idx < 0 || vt_idx >= max_vt_idx) vt_idx = -1; // ��Ч����������Ϊ������
        }
        if (parts.size() > 2 && !parts[2].empty()) {
            vn_idx = std::stoi(parts[2]) - 1;
            if (vn_idx < 0 || vn_idx >= max_vn_idx) vn_idx = -1; // ��Ч����������Ϊ������
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
    std::string currentObjectName = "default_object"; // OBJ �ļ����Բ�ָ��������

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        // �Ƴ����ܵĻس�����������β�ո�
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
            if (!(ss >> uv.x >> uv.y)) { // ����Ƿ�ɹ���ȡ��������
                std::cerr << "OBJECTMODEL WARNING: Malformed texture coordinate data at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            // float w_coord; // ��ѡ�� w
            // if (ss >> w_coord) { /* ����w */ }
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
                continue; // ����������
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
                    break; // ���������ʣ���������
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
                        int p_idx = std::get<0>(current_key); // ��ʱp_idx��֪����Ч��

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
            // ע�⣺ObjectModel ��ǰ�ṹֻ֧��һ������������б� (textureIDs)
            // ��ʵ�� usemtl �л���Ҫ��ģ�ͷָ�ɲ�ͬ�� Mesh��ÿ�� Mesh �����Լ��Ĳ���
            // ��������ֻ��¼�����һ�������� usemtl�������Լ��������� (��� textureIDs Ϊ��)
            if (!mtlBasePath.empty() && !currentMaterialName.empty() && textureIDs.empty()) {
                // ʵ��Ӧ���У�����Ӧ�ô��� .mtl �ļ��Ľ�����
                // ���� currentMaterialName ���� map_Kd (����������)
                // ���������ӡ����ʾ��Ҫ��һ������
                std::cout << "ObjectModel: Encountered usemtl '" << currentMaterialName
                    << "'. (mtllib: '" << materialLibraryFile << "'). "
                    << "MTL parsing and specific texture loading for this material is not yet implemented in this basic loader."
                    << std::endl;

                // ʾ�����������Ӳ��������Լ������һ������
                // (����Ҫ��ʵ�ʵ������ļ�������֪����������)
                // std::string exampleTextureName = currentMaterialName + "_diffuse.png"; // ���������Լ��
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
            // �����������������ڵ�ǰ�� ObjectModel (��һ����ṹ)��
            // ���ǿ��Լ�¼��һ�������������ߺ�����Щָ�
            if (currentObjectName == "default_object") { // ֻ��¼��һ��������
                ss >> currentObjectName;
            }
        }
        // 's' (smoothing group) ������ָ�����
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



    // λ��

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // ���㷨��

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // ������������

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));



    glBindVertexArray(0);

}



void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // ������������ (����ֻ��һ����������������)
    // ������Ҫ�����ӵ��߼���������������û����������
    if (!textureIDs.empty()) {
        glActiveTexture(GL_TEXTURE0); // ��������Ԫ0
        glBindTexture(GL_TEXTURE_2D, textureIDs[0]); // �󶨴�ģ�͵�����
        // ȷ����ɫ���е� sampler2D uniform ����Ϊ0
        // glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse1"), 0); // ͨ���ڳ�ʼ��ʱ����һ��
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    if (!textureIDs.empty()) {
        glBindTexture(GL_TEXTURE_2D, 0); // �������
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
        stbi_image_free(data); // data ��ʱΪnullptr��������Ҳ�޺�
        glDeleteTextures(1, &textureID);
        return 0;
    }
    return textureID;
}