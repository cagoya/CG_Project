#include "OurObjLoader.h" // 注意这里是 OurObjLoader.h
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <tuple> // 用于 std::tuple 作为 std::map 的键

// 辅助函数：分割字符串 (如果你的项目中没有通用的分割函数)
static inline std::vector<std::string> splitString_OurObj(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

OurObjLoader::OurObjLoader() {}

OurObjLoader::~OurObjLoader() {}

void OurObjLoader::parseFaceVertex(const std::string& faceVertexStr, int& v_idx, int& vt_idx, int& vn_idx) {
    v_idx = vt_idx = vn_idx = -1; // 默认索引为无效

    std::vector<std::string> parts = splitString_OurObj(faceVertexStr, '/');

    if (parts.empty()) return;

    if (!parts[0].empty()) v_idx = std::stoi(parts[0]) - 1; // OBJ 是 1-based, 转为 0-based

    if (parts.size() > 1 && !parts[1].empty()) {
        vt_idx = std::stoi(parts[1]) - 1;
    }
    if (parts.size() > 2 && !parts[2].empty()) {
        vn_idx = std::stoi(parts[2]) - 1;
    }
}

bool OurObjLoader::loadObj(const std::string& path,
    std::vector<OurObjMesh>& outMeshes,
    std::vector<std::string>& outMaterialLibs) {
    temp_positions.clear();
    temp_texCoords.clear();
    temp_normals.clear();
    outMeshes.clear();
    outMaterialLibs.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "OUR OBJ LOADER ERROR: Cannot open file: " << path << std::endl;
        return false;
    }

    std::string currentObjectName = "default_object";
    std::string currentMaterialName = "default_material"; // 或者 "" 表示无材质

    // 为当前正在构建的网格创建一个实例
    // 当材质或对象/组改变时，我们会将旧的 currentMesh 存入 outMeshes，并重置 currentMesh
    OurObjMesh currentMesh;
    currentMesh.name = currentObjectName;
    currentMesh.materialName = currentMaterialName;

    // 用于缓存和重用顶点 (v_idx, vt_idx, vn_idx) -> final_vertex_index
    // 这个缓存是针对 currentMesh 的，切换 Mesh 时需要清空或重新考虑
    std::map<std::tuple<int, int, int>, unsigned int> vertexCache;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "#" || keyword.empty()) {
            continue;
        }
        else if (keyword == "v") {
            glm::vec3 pos;
            if (!(ss >> pos.x >> pos.y >> pos.z)) {
                std::cerr << "OUR OBJ LOADER WARNING: Malformed vertex data at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            temp_positions.push_back(pos);
        }
        else if (keyword == "vt") {
            glm::vec2 uv;
            if (!(ss >> uv.x >> uv.y)) {
                std::cerr << "OUR OBJ LOADER WARNING: Malformed texture coordinate data at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            // uv.y = 1.0f - uv.y; // 通常需要翻转V轴，这里先不翻转，由使用方决定或在存入Vertex时处理
            temp_texCoords.push_back(uv);
        }
        else if (keyword == "vn") {
            glm::vec3 norm;
            if (!(ss >> norm.x >> norm.y >> norm.z)) {
                std::cerr << "OUR OBJ LOADER WARNING: Malformed vertex normal data at line " << lineNumber << ": " << line << std::endl;
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
                 std::cerr << "OUR OBJ LOADER WARNING: Face with less than 3 vertices at line " << lineNumber << ": " << line << std::endl;
                continue;
            }

            // 三角化 (扇形三角剖分)
            int v_idx0, vt_idx0, vn_idx0;
            parseFaceVertex(faceVertexStrings[0], v_idx0, vt_idx0, vn_idx0);

            for (size_t i = 1; i < faceVertexStrings.size() - 1; ++i) {
                int v_idx1, vt_idx1, vn_idx1;
                int v_idx2, vt_idx2, vn_idx2;

                parseFaceVertex(faceVertexStrings[i], v_idx1, vt_idx1, vn_idx1);
                parseFaceVertex(faceVertexStrings[i + 1], v_idx2, vt_idx2, vn_idx2);

                std::tuple<int, int, int> face_indices_tuples[3] = {
                    std::make_tuple(v_idx0, vt_idx0, vn_idx0),
                    std::make_tuple(v_idx1, vt_idx1, vn_idx1),
                    std::make_tuple(v_idx2, vt_idx2, vn_idx2)
                };

                for (int j = 0; j < 3; ++j) {
                    std::tuple<int, int, int> current_key = face_indices_tuples[j];

                    int p_idx = std::get<0>(current_key);
                    int t_idx = std::get<1>(current_key);
                    int n_idx = std::get<2>(current_key);

                    // 检查索引有效性
                    if (p_idx < 0 || static_cast<size_t>(p_idx) >= temp_positions.size() ||
                        (t_idx != -1 && (t_idx < 0 || static_cast<size_t>(t_idx) >= temp_texCoords.size())) ||
                        (n_idx != -1 && (n_idx < 0 || static_cast<size_t>(n_idx) >= temp_normals.size()))) {
                        std::cerr << "OUR OBJ LOADER WARNING: Invalid index in face definition at line " << lineNumber << std::endl;
                        // 可能需要更复杂的错误处理，比如跳过这个面或整个mesh
                        goto next_face_iteration; // 跳过这个损坏的面的剩余顶点（如果这是一个循环的一部分）
                        // 或者标记错误并继续解析其他部分
                    }


                    if (vertexCache.find(current_key) == vertexCache.end()) {
                        OurObjVertex new_vertex;
                        new_vertex.position = temp_positions[p_idx];

                        if (t_idx != -1) {
                            new_vertex.texCoords = temp_texCoords[t_idx];
                            new_vertex.texCoords.y = 1.0f - new_vertex.texCoords.y; // 翻转V
                        }
                        else {
                            new_vertex.texCoords = glm::vec2(0.0f, 0.0f);
                        }

                        if (n_idx != -1) {
                            new_vertex.normal = temp_normals[n_idx];
                        }
                        else {
                            new_vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f); // 标记为待计算或默认值
                        }

                        currentMesh.vertices.push_back(new_vertex);
                        unsigned int new_final_idx = static_cast<unsigned int>(currentMesh.vertices.size() - 1);
                        vertexCache[current_key] = new_final_idx;
                        currentMesh.indices.push_back(new_final_idx);
                    }
                    else {
                        currentMesh.indices.push_back(vertexCache[current_key]);
                    }
                }
            }
        next_face_iteration:; // 标签用于 goto
        }
        else if (keyword == "mtllib") {
            std::string mtlFilename;
            ss >> mtlFilename;
            outMaterialLibs.push_back(mtlFilename);
        }
        else if (keyword == "usemtl") {
            std::string newMaterialName;
            if (!(ss >> newMaterialName)) {
                std::cerr << "OUR OBJ LOADER WARNING: Malformed usemtl at line " << lineNumber << ": " << line << std::endl;
                continue;
            }

            if (currentMesh.materialName != newMaterialName && !currentMesh.vertices.empty()) {
                outMeshes.push_back(currentMesh); // 保存旧的 mesh
                currentMesh = OurObjMesh();       // 开始新的 mesh
                currentMesh.name = currentObjectName; // 沿用当前对象名
                vertexCache.clear();              // 新 mesh 使用新的顶点缓存
            }
            currentMesh.materialName = newMaterialName;
        }
        else if (keyword == "o" || keyword == "g") {
            std::string newName;
            if (!(ss >> newName)) {
                std::cerr << "OUR OBJ LOADER WARNING: Malformed o/g name at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            if (currentMesh.name != newName && !currentMesh.vertices.empty()) {
                outMeshes.push_back(currentMesh);
                currentMesh = OurObjMesh();
                vertexCache.clear();
            }
            currentMesh.name = newName;
            currentObjectName = newName; // 更新当前对象名，以便新的 usemtl 或 f 知道它属于哪个对象/组
        }
    }
    file.close();

    if (!currentMesh.vertices.empty()) {
        outMeshes.push_back(currentMesh);
    }

    if (outMeshes.empty() && temp_positions.empty()) {
        std::cout << "OUR OBJ LOADER: No geometric data or meshes were loaded from file: " << path << std::endl;
        // return false; // 可以根据情况决定是否算加载失败
    }
    else {
        std::cout << "OUR OBJ LOADER: Successfully parsed " << path << std::endl;
        std::cout << "  - Found " << temp_positions.size() << " raw geometric vertices." << std::endl;
        std::cout << "  - Found " << temp_texCoords.size() << " raw texture coordinates." << std::endl;
        std::cout << "  - Found " << temp_normals.size() << " raw vertex normals." << std::endl;
        std::cout << "  - Generated " << outMeshes.size() << " meshes." << std::endl;
        for (size_t i = 0; i < outMeshes.size(); ++i) {
            const auto& mesh = outMeshes[i];
            std::cout << "    - Mesh " << i << " (Name: \"" << mesh.name
                << "\", Material: \"" << mesh.materialName << "\"): "
                << mesh.vertices.size() << " final vertices, "
                << mesh.indices.size() << " indices." << std::endl;
        }
    }
    return true;
}