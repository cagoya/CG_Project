#include "OurObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <tuple>

// STB Image (ȷ�������ֻ��һ�����뵥Ԫ�ж��壬����ദʹ��stb_image.h)
#ifndef STB_IMAGE_IMPLEMENTATION // ͨ������һ��ר�ŵ� .c �� .cpp �ļ�������ȷ��ֻ�����ļ�����һ��
#include "stb_image.h" // ���ڼ�������ͼƬ
#define STB_IMAGE_IMPLEMENTATION
#endif

// �����������ָ��ַ���
static inline std::vector<std::string> splitString_OurObj(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

 bool OurObjLoader::objParseFaceVertexIndices(const std::string& faceVertexStr,
                                            int& v_idx, int& vt_idx, int& vn_idx,
                                            size_t max_v, size_t max_vt, size_t max_vn) {
     v_idx = vt_idx = vn_idx = -1; //Ĭ��ֵ��ʾȱʧ
     std::vector<std::string> parts = splitString_OurObj(faceVertexStr, '/');
     try {
         if (parts.size() > 0 && !parts[0].empty()) v_idx = std::stoi(parts[0]) - 1;
         if (parts.size() > 1 && !parts[1].empty()) vt_idx = std::stoi(parts[1]) - 1;
         if (parts.size() > 2 && !parts[2].empty()) vn_idx = std::stoi(parts[2]) - 1;

         // ����������Ч�Լ�� (ע��OBJ������1���ģ�����ת��Ϊ0��)
         if (v_idx >= static_cast<int>(max_v) || v_idx < -1) { /*std::cerr << "Invalid v index" <<std::endl;*/ return false; }
         if (vt_idx >= static_cast<int>(max_vt) || vt_idx < -1) { /*std::cerr << "Invalid vt index" <<std::endl;*/ return false; }
         if (vn_idx >= static_cast<int>(max_vn) || vn_idx < -1) { /*std::cerr << "Invalid vn index" <<std::endl;*/ return false; }
         return true;

     } catch (const std::exception& e) {
         std::cerr << "OUR OBJ LOADER WARNING: Error parsing face vertex component '" << faceVertexStr << "': " << e.what() << std::endl;
         v_idx = vt_idx = vn_idx = -1;
         return false;
     }
 }


void OurObjLoader::parseFaceVertex(const std::string& faceVertexStr, int& v_idx, int& vt_idx, int& vn_idx) {
    v_idx = vt_idx = vn_idx = -1;
    std::vector<std::string> parts = splitString_OurObj(faceVertexStr, '/');
    if (parts.empty()) return;
    try {
        if (!parts[0].empty()) v_idx = std::stoi(parts[0]) - 1;
        if (parts.size() > 1 && !parts[1].empty()) vt_idx = std::stoi(parts[1]) - 1;
        if (parts.size() > 2 && !parts[2].empty()) vn_idx = std::stoi(parts[2]) - 1;
    }
    catch (const std::exception& e) {
        std::cerr << "OUR OBJ LOADER WARNING: Error parsing face vertex component '" << faceVertexStr << "': " << e.what() << std::endl;
        v_idx = vt_idx = vn_idx = -1; // �����Է����ֽ���
    }
}

// �޸ģ�mtlFileBasePath ����������Ҫ����Ϊ����·���������� mtlFileBasePath ����
bool OurObjLoader::parseMtlFile(const std::string& mtlFilePath,
    std::map<std::string, MaterialInfo>& materials) {
    std::ifstream mtlFile(mtlFilePath);
    if (!mtlFile.is_open()) {
        std::cerr << "OUR OBJ LOADER ERROR: Cannot open MTL file: " << mtlFilePath << std::endl;
        return false;
    }
    std::cout << "OUR OBJ LOADER: Parsing MTL file: " << mtlFilePath << std::endl;

    MaterialInfo currentMaterial;
    std::string currentMaterialNameKey;
    bool materialActive = false;
    std::string line;

    while (std::getline(mtlFile, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        while (!line.empty() && isspace(line.back())) line.pop_back();
        while (!line.empty() && isspace(line.front())) line.erase(0, 1);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "newmtl") {
            if (materialActive && !currentMaterialNameKey.empty()) {
                materials[currentMaterialNameKey] = currentMaterial;
            }
            if (!(ss >> currentMaterialNameKey)) { /* error handling */ continue; }
            currentMaterial = MaterialInfo();
            currentMaterial.name = currentMaterialNameKey;
            materialActive = true;
        }
        else if (!materialActive) {
            continue;
        }
        else if (keyword == "Ka") {
            ss >> currentMaterial.ambientColor.r >> currentMaterial.ambientColor.g >> currentMaterial.ambientColor.b;
        }
        else if (keyword == "Kd") {
            ss >> currentMaterial.diffuseColor.r >> currentMaterial.diffuseColor.g >> currentMaterial.diffuseColor.b;
        }
        else if (keyword == "Ks") {
            ss >> currentMaterial.specularColor.r >> currentMaterial.specularColor.g >> currentMaterial.specularColor.b;
        }
        else if (keyword == "Ns") {
            ss >> currentMaterial.shininess;
        }
        else if (keyword == "d") {
            ss >> currentMaterial.dissolve;
        }
        else if (keyword == "Tr") {
            float transparency;
            ss >> transparency;
            currentMaterial.dissolve = 1.0f - transparency;
        }
        else if (keyword == "map_Kd") {
            std::string texFilename;
            std::getline(ss, texFilename);
            while (!texFilename.empty() && isspace(texFilename.front())) texFilename.erase(0, 1);
            currentMaterial.diffuseTextureMap = texFilename;
        }
        // ������Ӷ����� map_ (�� map_Ks, map_Bump) �Ľ���
    }
    if (materialActive && !currentMaterialNameKey.empty()) {
        materials[currentMaterialNameKey] = currentMaterial;
    }
    mtlFile.close();
    return true;
}

// �����ļ���ʵ��
GLuint OurObjLoader::loadTextureFromFile(const char* path) {
    GLuint textureID = 0;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    // stbi_set_flip_vertically_on_load(true); // ������������µߵ��ģ��������ȫ������
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;
        else {
            std::cerr << "Texture " << path << ": Unknown number of components: " << nrComponents << std::endl;
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

        std::cout << "OUR OBJ LOADER: Successfully loaded texture: " << path << " (ID: " << textureID << ")" << std::endl;
    }
    else {
        std::cerr << "OUR OBJ LOADER ERROR: Texture failed to load at path: " << path << " (stb_image error: " << stbi_failure_reason() << ")" << std::endl;
        glDeleteTextures(1, &textureID); // ɾ��ʧ�ܵ��������
        textureID = 0; // ����0��ʾʧ��
    }
    stbi_image_free(data); // �����ͷ�stb_image���ص��ڴ�
    return textureID;
}


// �޸� loadObj ������ʹ�� m_loadedMaterials �� m_textureCache
bool OurObjLoader::loadObj(const std::string& objFilePath,
    const std::string& mtlFileBasePath,
    std::vector<OurObjMesh>& outMeshes) {
    temp_positions.clear();
    temp_texCoords.clear();
    temp_normals.clear();
    outMeshes.clear();
    m_loadedMaterials.clear(); // ���֮ǰ���صĲ���
    m_textureCache.clear();    // ���������

    std::ifstream file(objFilePath);
    if (!file.is_open()) {
        std::cerr << "OUR OBJ LOADER ERROR: Cannot open file: " << objFilePath << std::endl;
        return false;
    }

    std::string currentObjectName = "default_object";
    std::string currentMaterialName = "";

    OurObjMesh currentMesh;
    currentMesh.name = currentObjectName;
    currentMesh.materialName = currentMaterialName;
    std::map<std::tuple<int, int, int>, unsigned int> vertexCache;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        while (!line.empty() && isspace(line.back())) line.pop_back();
        while (!line.empty() && isspace(line.front())) line.erase(0, 1);

        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword.empty() || keyword[0] == '#') {
            continue;
        }
        else if (keyword == "mtllib") {
            std::string mtlFilename;
            if (ss >> mtlFilename) {
                std::cout << "DEBUG: ss: " << ss.str() << std::endl;
                std::cout << "DEBUG: mtllib - mtlFileBasePath from arg: [" << mtlFileBasePath << "]" << std::endl;
                std::cout << "DEBUG: mtllib - mtlFilename read from OBJ: [" << mtlFilename << "]" << std::endl;
                std::string fullMtlPath = mtlFileBasePath;
                if (!fullMtlPath.empty() && fullMtlPath.back() != '/' && fullMtlPath.back() != '\\') {
                    fullMtlPath += '/';
                }
                fullMtlPath += mtlFilename;
                std::cout << "DEBUG: mtllib - fullMtlPath constructed: [" << fullMtlPath << "]" << std::endl; // ��ӡ��Ϻ��·��

                parseMtlFile(fullMtlPath, m_loadedMaterials); // ʹ�ó�Ա���� m_loadedMaterials
            }
            else { /* error handling */ }
        }
        else if (keyword == "usemtl") {
            std::string newMaterialName;
            if (!(ss >> newMaterialName)) { /* error handling */ continue; }

            if (currentMaterialName != newMaterialName && !currentMesh.vertices.empty()) {
                outMeshes.push_back(currentMesh);
                currentMesh = OurObjMesh();
                currentMesh.name = currentObjectName;
                vertexCache.clear();
            }
            currentMesh.materialName = newMaterialName;
            currentMaterialName = newMaterialName;

            // Ϊ�µĲ��ʼ������� (�����δ����)
            auto matIt = m_loadedMaterials.find(currentMaterialName);
            if (matIt != m_loadedMaterials.end()) {
                const MaterialInfo& material = matIt->second;
                if (!material.diffuseTextureMap.empty()) {
                    std::string textureRelativePath = material.diffuseTextureMap;
                    // ����·��ͨ������� .mtl �ļ����� .mtl �ļ�·���ǻ��� mtlFileBasePath �ġ�
                    // ���� mtlFileBasePath ��������Դ�ĸ�Ŀ¼��
                    std::string fullTexturePath = mtlFileBasePath;
                    if (!fullTexturePath.empty() && fullTexturePath.back() != '/' && fullTexturePath.back() != '\\' &&
                        !textureRelativePath.empty() && textureRelativePath.front() != '/' && textureRelativePath.front() != '\\') {
                        fullTexturePath += '/';
                    }
                    fullTexturePath += textureRelativePath;

                    // ���������
                    auto texCacheIt = m_textureCache.find(fullTexturePath);
                    if (texCacheIt != m_textureCache.end()) {
                        currentMesh.diffuseTextureId = texCacheIt->second;
                        // std::cout << "Using cached texture ID " << currentMesh.diffuseTextureId << " for " << fullTexturePath << std::endl;
                    }
                    else {
                        GLuint texID = loadTextureFromFile(fullTexturePath.c_str());
                        if (texID != 0) {
                            currentMesh.diffuseTextureId = texID;
                            m_textureCache[fullTexturePath] = texID; // ���뻺��
                        }
                    }
                }
            }
        }
        else if (keyword == "o" || keyword == "g") {
            std::string newName;
            if (!(ss >> newName)) { /* error handling */ continue; }
            if (currentMesh.name != newName && !currentMesh.vertices.empty()) {
                outMeshes.push_back(currentMesh);
                currentMesh = OurObjMesh();
                currentMesh.materialName = currentMaterialName;
                vertexCache.clear();
            }
            currentMesh.name = newName;
            currentObjectName = newName;
        }
        else if (keyword == "v") {
            glm::vec3 pos;
            if (!(ss >> pos.x >> pos.y >> pos.z)) continue;
            temp_positions.push_back(pos);
        }
        else if (keyword == "vt") {
            glm::vec2 uv;
            if (!(ss >> uv.x >> uv.y)) continue;
            temp_texCoords.push_back(uv);
        }
        else if (keyword == "vn") {
            glm::vec3 norm;
            if (!(ss >> norm.x >> norm.y >> norm.z)) continue;
            temp_normals.push_back(norm);
        }
        else if (keyword == "f") { /* ʹ����Ч�� temp_* ������С����������� ... */
            std::vector<std::string> faceVertexStrings;
            std::string faceVertexToken;
            while (ss >> faceVertexToken) faceVertexStrings.push_back(faceVertexToken);

            if (faceVertexStrings.size() < 3) continue;

            int v_idx0, vt_idx0, vn_idx0;
            // ���� temp �����Ĵ�С�����������
            if (!objParseFaceVertexIndices(faceVertexStrings[0], v_idx0, vt_idx0, vn_idx0,
                temp_positions.size(), temp_texCoords.size(), temp_normals.size())) {
                continue;
            }

            for (size_t i = 1; i < faceVertexStrings.size() - 1; ++i) {
                int v_idx1, vt_idx1, vn_idx1;
                int v_idx2, vt_idx2, vn_idx2;
                bool v1_ok = objParseFaceVertexIndices(faceVertexStrings[i], v_idx1, vt_idx1, vn_idx1,
                    temp_positions.size(), temp_texCoords.size(), temp_normals.size());
                bool v2_ok = objParseFaceVertexIndices(faceVertexStrings[i + 1], v_idx2, vt_idx2, vn_idx2,
                    temp_positions.size(), temp_texCoords.size(), temp_normals.size());
                if (!v1_ok || !v2_ok) break;

                std::tuple<int, int, int> face_indices_tuples[3] = {
                    std::make_tuple(v_idx0, vt_idx0, vn_idx0),
                    std::make_tuple(v_idx1, vt_idx1, vn_idx1),
                    std::make_tuple(v_idx2, vt_idx2, vn_idx2)
                };

                for (int j = 0; j < 3; ++j) {
                    std::tuple<int, int, int> current_key = face_indices_tuples[j];
                    if (vertexCache.find(current_key) == vertexCache.end()) {
                        OurObjVertex new_vert;
                        int p_idx = std::get<0>(current_key);
                        new_vert.position = temp_positions[p_idx];
                        int t_idx = std::get<1>(current_key);
                        if (t_idx != -1 && static_cast<size_t>(t_idx) < temp_texCoords.size()) { // �ٴμ������
                            new_vert.texCoords = temp_texCoords[t_idx];
                            new_vert.texCoords.y = 1.0f - new_vert.texCoords.y;
                        }
                        else { new_vert.texCoords = glm::vec2(0.0f, 0.0f); }
                        int n_idx = std::get<2>(current_key);
                        if (n_idx != -1 && static_cast<size_t>(n_idx) < temp_normals.size()) { // �ٴμ������
                            new_vert.normal = temp_normals[n_idx];
                        }
                        else { new_vert.normal = glm::vec3(0.0f, 0.0f, 0.0f); }
                        currentMesh.vertices.push_back(new_vert);
                        unsigned int new_final_idx = static_cast<unsigned int>(currentMesh.vertices.size() - 1);
                        vertexCache[current_key] = new_final_idx;
                        currentMesh.indices.push_back(new_final_idx);
                    }
                    else {
                        currentMesh.indices.push_back(vertexCache[current_key]);
                    }
                }
            }
        }
    }
    file.close();

    if (!currentMesh.vertices.empty()) {
        outMeshes.push_back(currentMesh);
    }

    if (outMeshes.empty() && temp_positions.empty()) {
        std::cout << "OUR OBJ LOADER: No geometric data or meshes were loaded from file: " << objFilePath << std::endl;
    }
    return true;
}
OurObjLoader::OurObjLoader() {
}

OurObjLoader::~OurObjLoader() { 
}