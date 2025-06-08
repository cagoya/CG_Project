#include "OurObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <tuple>
#include <immintrin.h>  // 添加 SIMD 支持
#include <glad/gl.h>    // 添加 GLAD 支持

#ifndef STB_IMAGE_IMPLEMENTATION // 通常放在一个专门的 .c 或 .cpp 文件，或者确保只在主文件定义一次
#include "stb_image.h" // 用于加载纹理图片
#define STB_IMAGE_IMPLEMENTATION
#endif

// 辅助函数：分割字符串
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
     v_idx = vt_idx = vn_idx = -1; //默认值表示缺失
     std::vector<std::string> parts = splitString_OurObj(faceVertexStr, '/');
     try {
         if (parts.size() > 0 && !parts[0].empty()) v_idx = std::stoi(parts[0]) - 1;
         if (parts.size() > 1 && !parts[1].empty()) vt_idx = std::stoi(parts[1]) - 1;
         if (parts.size() > 2 && !parts[2].empty()) vn_idx = std::stoi(parts[2]) - 1;

         // 基础索引有效性检查 (注意OBJ索引是1基的，我们转换为0基)
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
        v_idx = vt_idx = vn_idx = -1; // 重置以防部分解析
    }
}

// 修改：mtlFileBasePath 参数不再需要，因为纹理路径将基于主 mtlFileBasePath 构建
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
        // 可以添加对其他 map_ (如 map_Ks, map_Bump) 的解析
    }
    if (materialActive && !currentMaterialNameKey.empty()) {
        materials[currentMaterialNameKey] = currentMaterial;
    }
    mtlFile.close();
    return true;
}

// 纹理文件的实现
GLuint OurObjLoader::loadTextureFromFile(const char* path) {
    GLuint textureID = 0;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    // stbi_set_flip_vertically_on_load(true); // 如果纹理是上下颠倒的，在这里或全局设置
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
        glDeleteTextures(1, &textureID); // 删除失败的纹理对象
        textureID = 0; // 返回0表示失败
    }
    stbi_image_free(data); // 总是释放stb_image加载的内存
    return textureID;
}

// 添加 SIMD 优化的顶点处理函数
void OurObjLoader::processVerticesSIMD(std::vector<glm::vec3>& vertices) {
    if (vertices.empty()) return;  // 添加空检查
    
    size_t count = vertices.size();
    size_t simdCount = count - (count % 4);  // 确保是4的倍数
    
    // 确保内存对齐
    for (size_t i = 0; i < simdCount; i += 4) {
        // 使用临时变量存储顶点数据
        glm::vec3 temp[4];
        for (int j = 0; j < 4; j++) {
            temp[j] = vertices[i + j];
        }
        
        // 加载4个顶点到SIMD寄存器
        __m128 v1 = _mm_set_ps(0.0f, temp[0].z, temp[0].y, temp[0].x);
        __m128 v2 = _mm_set_ps(0.0f, temp[1].z, temp[1].y, temp[1].x);
        __m128 v3 = _mm_set_ps(0.0f, temp[2].z, temp[2].y, temp[2].x);
        __m128 v4 = _mm_set_ps(0.0f, temp[3].z, temp[3].y, temp[3].x);
        
        // SIMD运算
        __m128 scale = _mm_set1_ps(1.0f);
        v1 = _mm_mul_ps(v1, scale);
        v2 = _mm_mul_ps(v2, scale);
        v3 = _mm_mul_ps(v3, scale);
        v4 = _mm_mul_ps(v4, scale);
        
        // 存储回临时变量
        float result1[4], result2[4], result3[4], result4[4];
        _mm_store_ps(result1, v1);
        _mm_store_ps(result2, v2);
        _mm_store_ps(result3, v3);
        _mm_store_ps(result4, v4);
        
        // 更新顶点数据
        for (int j = 0; j < 4; j++) {
            vertices[i + j].x = result1[j];
            vertices[i + j].y = result2[j];
            vertices[i + j].z = result3[j];
        }
    }
    
    // 处理剩余的顶点
    for (size_t i = simdCount; i < count; i++) {
        vertices[i] *= 1.0f;
    }
}

// 修改 loadObj 方法以使用 GPU 处理
bool OurObjLoader::loadObj(const std::string& objFilePath,
    const std::string& mtlFileBasePath,
    std::vector<OurObjMesh>& outMeshes) {
    temp_positions.clear();
    temp_texCoords.clear();
    temp_normals.clear();
    outMeshes.clear();
    m_loadedMaterials.clear(); // 清空之前加载的材质
    m_textureCache.clear();    // 清空纹理缓存

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
                
                // 修复MTL文件路径解析
                std::string fullMtlPath = mtlFileBasePath;
                if (!fullMtlPath.empty() && fullMtlPath.back() != '/' && fullMtlPath.back() != '\\') {
                    fullMtlPath += '/';
                }
                fullMtlPath += mtlFilename;
                std::cout << "DEBUG: mtllib - fullMtlPath constructed: [" << fullMtlPath << "]" << std::endl;

                if (!parseMtlFile(fullMtlPath, m_loadedMaterials)) {
                    std::cerr << "OUR OBJ LOADER ERROR: Failed to parse MTL file: " << fullMtlPath << std::endl;
                    return false;
                }
            }
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

            // 为新的材质加载纹理 (如果尚未加载)
            auto matIt = m_loadedMaterials.find(currentMaterialName);
            if (matIt != m_loadedMaterials.end()) {
                const MaterialInfo& material = matIt->second;
                if (!material.diffuseTextureMap.empty()) {
                    std::string textureRelativePath = material.diffuseTextureMap;
                    // 纹理路径通常相对于 .mtl 文件，而 .mtl 文件路径是基于 mtlFileBasePath 的。
                    // 假设 mtlFileBasePath 是所有资源的根目录。
                    std::string fullTexturePath = mtlFileBasePath;
                    if (!fullTexturePath.empty() && fullTexturePath.back() != '/' && fullTexturePath.back() != '\\' &&
                        !textureRelativePath.empty() && textureRelativePath.front() != '/' && textureRelativePath.front() != '\\') {
                        fullTexturePath += '/';
                    }
                    fullTexturePath += textureRelativePath;

                    // 检查纹理缓存
                    auto texCacheIt = m_textureCache.find(fullTexturePath);
                    if (texCacheIt != m_textureCache.end()) {
                        currentMesh.diffuseTextureId = texCacheIt->second;
                        // std::cout << "Using cached texture ID " << currentMesh.diffuseTextureId << " for " << fullTexturePath << std::endl;
                    }
                    else {
                        GLuint texID = loadTextureFromFile(fullTexturePath.c_str());
                        if (texID != 0) {
                            currentMesh.diffuseTextureId = texID;
                            m_textureCache[fullTexturePath] = texID; // 加入缓存
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
        else if (keyword == "f") { /* 使用有效的 temp_* 容器大小进行索引检查 ... */
            std::vector<std::string> faceVertexStrings;
            std::string faceVertexToken;
            while (ss >> faceVertexToken) faceVertexStrings.push_back(faceVertexToken);

            if (faceVertexStrings.size() < 3) continue;

            int v_idx0, vt_idx0, vn_idx0;
            // 传递 temp 容器的大小用于索引检查
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
                        if (t_idx != -1 && static_cast<size_t>(t_idx) < temp_texCoords.size()) { // 再次检查索引
                            new_vert.texCoords = temp_texCoords[t_idx];
                            new_vert.texCoords.y = 1.0f - new_vert.texCoords.y;
                        }
                        else { new_vert.texCoords = glm::vec2(0.0f, 0.0f); }
                        int n_idx = std::get<2>(current_key);
                        if (n_idx != -1 && static_cast<size_t>(n_idx) < temp_normals.size()) { // 再次检查索引
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

    // 在解析完顶点数据后，使用 SIMD 处理
    if (!temp_positions.empty()) {
        processVerticesSIMD(temp_positions);
    }

    // 创建 GPU 缓冲区
    GLuint vbo;
    glGenBuffers(1, &vbo);
    if (vbo == 0) {
        std::cerr << "Failed to generate vertex buffer object" << std::endl;
        return false;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // 计算总顶点数据大小
    size_t totalVertices = temp_positions.size();
    if (totalVertices == 0) {
        std::cerr << "No vertices to process" << std::endl;
        glDeleteBuffers(1, &vbo);
        return false;
    }
    
    size_t vertexDataSize = totalVertices * (3 + 2 + 3) * sizeof(float);  // 位置(3) + 纹理坐标(2) + 法线(3)
    
    // 分配 GPU 内存
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, nullptr, GL_STATIC_DRAW);
    
    // 使用映射缓冲区进行数据上传
    void* bufferData = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (!bufferData) {
        std::cerr << "Failed to map buffer" << std::endl;
        glDeleteBuffers(1, &vbo);
        return false;
    }
    
    float* data = static_cast<float*>(bufferData);
    size_t offset = 0;
    
    // 使用 SIMD 优化的方式复制数据
    for (size_t i = 0; i < totalVertices; i += 4) {
        size_t remaining = std::min(4ull, totalVertices - i);
        
        // 复制位置数据
        for (size_t j = 0; j < remaining; j++) {
            if (i + j < temp_positions.size()) {
                data[offset++] = temp_positions[i + j].x;
                data[offset++] = temp_positions[i + j].y;
                data[offset++] = temp_positions[i + j].z;
            }
        }
        
        // 复制纹理坐标
        for (size_t j = 0; j < remaining; j++) {
            if (i + j < temp_texCoords.size()) {
                data[offset++] = temp_texCoords[i + j].x;
                data[offset++] = temp_texCoords[i + j].y;
            } else {
                data[offset++] = 0.0f;
                data[offset++] = 0.0f;
            }
        }
        
        // 复制法线数据
        for (size_t j = 0; j < remaining; j++) {
            if (i + j < temp_normals.size()) {
                data[offset++] = temp_normals[i + j].x;
                data[offset++] = temp_normals[i + j].y;
                data[offset++] = temp_normals[i + j].z;
            } else {
                data[offset++] = 0.0f;
                data[offset++] = 0.0f;
                data[offset++] = 0.0f;
            }
        }
    }
    
    if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
        std::cerr << "Failed to unmap buffer" << std::endl;
        glDeleteBuffers(1, &vbo);
        return false;
    }
    
    // 设置顶点属性指针
    size_t stride = (3 + 2 + 3) * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    return true;
}

OurObjLoader::OurObjLoader() {
}

OurObjLoader::~OurObjLoader() { 
}