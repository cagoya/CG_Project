#include "OurObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <tuple>
#include <immintrin.h>  // 添加 SIMD 支持
#include <glad/gl.h>    // 添加 GLAD 支持
#include <cstdlib> // 为了使用 atol

#ifndef STB_IMAGE_IMPLEMENTATION // 通常放在一个专门的 .c 或 .cpp 文件，或者确保只在主文件定义一次
#include "stb_image.h" // 用于加载纹理图片
#define STB_IMAGE_IMPLEMENTATION
#endif

static inline const char* fast_parse_face_component(const char* s, int& val) {
    while (*s && isspace(*s)) s++;
    val = atol(s);
    while (*s && isdigit(*s)) s++;
    return s;
}
static inline std::vector<std::string> splitString_OurObj(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

static inline bool fast_parse_face_vertex(const char*& s, int& v, int& t, int& n) {
    v = t = n = 0; // 重置为0，我们将用它来存储1基索引

    while (*s && isspace(*s)) s++;
    if (!*s || !isdigit(*s)) return false; // 必须以数字开头
    while (*s && isdigit(*s)) v = v * 10 + (*s++ - '0');

    if (*s != '/') return true; // 格式是 f v v v
    s++; // 跳过第一个 '/'
    // 解析第二个数字 (纹理索引)
    if (*s != '/') {
        while (*s && isdigit(*s)) t = t * 10 + (*s++ - '0');
    }
    if (*s != '/') return true; // 格式是 f v/vt v/vt v/vt
    s++; // 跳过第二个 '/'
    // 解析第三个数字 (法线索引)
    while (*s && isdigit(*s)) n = n * 10 + (*s++ - '0');

    return true;
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
        // glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    else {
        std::cerr << "OUR OBJ LOADER ERROR: Texture failed to load at path: " << path << " (stb_image error: " << stbi_failure_reason() << ")" << std::endl;
        glDeleteTextures(1, &textureID); // 删除失败的纹理对象
        textureID = 0; // 返回0表示失败
    }
    stbi_image_free(data); // 总是释放stb_image加载的内存
    return textureID;
}


// 在 OurObjLoader.cpp 中

// --- 使用最终修正版的SIMD代码替换旧的 calculateNormals 函数 ---
void calculateNormals(OurObjMesh& mesh) {
    if (mesh.vertices.empty() || mesh.indices.empty()) {
        return;
    }

    std::vector<glm::vec3> temp_normals(mesh.vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

    // --- 第一部分：累加面法线 (这部分保持不变) ---
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        unsigned int i0 = mesh.indices[i];
        unsigned int i1 = mesh.indices[i + 1];
        unsigned int i2 = mesh.indices[i + 2];
        const glm::vec3& v0 = mesh.vertices[i0].position;
        const glm::vec3& v1 = mesh.vertices[i1].position;
        const glm::vec3& v2 = mesh.vertices[i2].position;
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 face_normal = glm::cross(edge2, edge1);
        temp_normals[i0] += face_normal;
        temp_normals[i1] += face_normal;
        temp_normals[i2] += face_normal;
    }

    // --- 第二部分：使用SIMD批量归一化所有法线 ---
    size_t i = 0;
    for (; i + 3 < temp_normals.size(); i += 4) {
        // 1. 转置加载4个vec3到3个__m128寄存器 (SoA)
        __m128 v0 = _mm_loadu_ps(&temp_normals[i].x);
        __m128 v1 = _mm_loadu_ps(&temp_normals[i+1].x);
        __m128 v2 = _mm_loadu_ps(&temp_normals[i+2].x);
        __m128 v3 = _mm_loadu_ps(&temp_normals[i+3].x);
        _MM_TRANSPOSE4_PS(v0, v1, v2, v3);

        __m128 x_vals = v0;
        __m128 y_vals = v1;
        __m128 z_vals = v2;

        // 2. 计算长度的平方
        __m128 x2 = _mm_mul_ps(x_vals, x_vals);
        __m128 y2 = _mm_mul_ps(y_vals, y_vals);
        __m128 z2 = _mm_mul_ps(z_vals, z_vals);
        __m128 length_sq = _mm_add_ps(_mm_add_ps(x2, y2), z2);

        // 3. 计算长度的倒数
        __m128 inv_length = _mm_rsqrt_ps(length_sq);

        // 4. 完成归一化
        x_vals = _mm_mul_ps(x_vals, inv_length);
        y_vals = _mm_mul_ps(y_vals, inv_length);
        z_vals = _mm_mul_ps(z_vals, inv_length);

        // 5. 将数据转置回来
        _MM_TRANSPOSE4_PS(x_vals, y_vals, z_vals, v3);

        // +++ 6. 安全地将结果从SIMD寄存器存回内存 +++
        // 创建一个临时的、16字节对齐的数组来接收SIMD的写入
        alignas(16) float results[4][4];
        _mm_store_ps(results[0], x_vals);
        _mm_store_ps(results[1], y_vals);
        _mm_store_ps(results[2], z_vals);
        _mm_store_ps(results[3], v3);

        // 从临时数组中，只把我们需要的x,y,z分量拷贝回去，完美避免内存溢出
        mesh.vertices[i+0].normal = glm::vec3(results[0][0], results[0][1], results[0][2]);
        mesh.vertices[i+1].normal = glm::vec3(results[1][0], results[1][1], results[1][2]);
        mesh.vertices[i+2].normal = glm::vec3(results[2][0], results[2][1], results[2][2]);
        mesh.vertices[i+3].normal = glm::vec3(results[3][0], results[3][1], results[3][2]);
    }

    // --- 处理剩余的、不足4个的向量 (收尾工作) ---
    for (; i < temp_normals.size(); ++i) {
        mesh.vertices[i].normal = glm::normalize(temp_normals[i]);
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

                // 修复MTL文件路径解析
                std::string fullMtlPath = mtlFileBasePath;
                if (!fullMtlPath.empty() && fullMtlPath.back() != '/' && fullMtlPath.back() != '\\') {
                    fullMtlPath += '/';
                }
                fullMtlPath += mtlFilename;

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
        else if (keyword == "f") {
            std::string face_line;
            std::getline(ss, face_line); // 读取 'f' 之后的所有内容
            const char* p = face_line.c_str();

            // 用数组来存储一行面数据中所有顶点的索引 (最多支持四边形)
            int face_v_indices[4], face_vt_indices[4], face_vn_indices[4];
            int vertex_count = 0;

            // 循环解析一行中的所有 v/vt/vn 组合
            while (*p && vertex_count < 4) {
                // 使用我们之前添加的快速C风格解析函数
                p = fast_parse_face_component(p, face_v_indices[vertex_count]);
                face_vt_indices[vertex_count] = 0; // 默认值为0 (代表不存在)
                face_vn_indices[vertex_count] = 0; // 默认值为0 (代表不存在)

                if (*p == '/') {
                    p++;
                    if (*p != '/') {
                        p = fast_parse_face_component(p, face_vt_indices[vertex_count]);
                    }
                    if (*p == '/') {
                        p++;
                        p = fast_parse_face_component(p, face_vn_indices[vertex_count]);
                    }
                }
                vertex_count++;
            }

            if (vertex_count < 3) continue; // 如果顶点数少于3，不是一个有效的面，跳过

            // 将面进行三角化处理 (这样代码也能正确处理四边形面)
            for (int i = 1; i < vertex_count - 1; ++i) {

                // *** 这里我们定义并初始化了每个三角形的索引元组 ***
                std::tuple<int, int, int> triangle_vertex_keys[3] = {
                    // OBJ索引是1基的，我们在这里统一转换为0基
                    std::make_tuple(face_v_indices[0] - 1,   face_vt_indices[0] - 1,   face_vn_indices[0] - 1),
                    std::make_tuple(face_v_indices[i] - 1,   face_vt_indices[i] - 1,   face_vn_indices[i] - 1),
                    std::make_tuple(face_v_indices[i+1] - 1, face_vt_indices[i+1] - 1, face_vn_indices[i+1] - 1)
                };

                // 后续的 vertexCache 逻辑保持不变，它使用我们刚刚定义的 triangle_vertex_keys
                for (int j = 0; j < 3; ++j) {
                    std::tuple<int, int, int> current_key = triangle_vertex_keys[j];
                    if (vertexCache.find(current_key) == vertexCache.end()) {
                        OurObjVertex new_vert;
                        int p_idx = std::get<0>(current_key);
                        if (p_idx >= 0 && static_cast<size_t>(p_idx) < temp_positions.size()) {
                             new_vert.position = temp_positions[p_idx];
                        }

                        int t_idx = std::get<1>(current_key);
                        if (t_idx >= 0 && static_cast<size_t>(t_idx) < temp_texCoords.size()) {
                            new_vert.texCoords = temp_texCoords[t_idx];
                        } else {
                            new_vert.texCoords = glm::vec2(0.0f, 0.0f);
                        }

                        int n_idx = std::get<2>(current_key);
                        if (n_idx >= 0 && static_cast<size_t>(n_idx) < temp_normals.size()) {
                            new_vert.normal = temp_normals[n_idx];
                        } else {
                            new_vert.normal = glm::vec3(0.0f, 0.0f, 0.0f);
                        }

                        currentMesh.vertices.push_back(new_vert);
                        unsigned int new_final_idx = static_cast<unsigned int>(currentMesh.vertices.size() - 1);
                        vertexCache[current_key] = new_final_idx;
                        currentMesh.indices.push_back(new_final_idx);
                    } else {
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
    for (auto& mesh : outMeshes) {
        // 调用我们上面创建的辅助函数
        calculateNormals(mesh);
    }
    // std::cout << "OUR OBJ LOADER: Normal calculation complete." << std::endl;
    return true;
}

OurObjLoader::OurObjLoader() {
}

OurObjLoader::~OurObjLoader() { 
}