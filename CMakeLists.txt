cmake_minimum_required(VERSION 3.20)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "utility")

project(cg_projects LANGUAGES C CXX VERSION 1.1)

# 编译器设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 构建类型默认 Release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Windows 特定宏定义
if (WIN32)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

# 拷贝资源目录
file(COPY "media/" DESTINATION "media")

# === 添加依赖库 ===
# glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(./external/glfw)
set_target_properties(glfw PROPERTIES FOLDER "lib/glfw")
set_target_properties(uninstall PROPERTIES FOLDER "lib/glfw")
set_target_properties(update_mappings PROPERTIES FOLDER "lib/glfw")

# glad
add_subdirectory(./external/glad)
set_target_properties(glad PROPERTIES FOLDER "lib")

# glm
add_subdirectory(./external/glm)
set_target_properties(glm PROPERTIES FOLDER "lib/glm")

# stb
add_subdirectory(./external/stb)
set_target_properties(stb PROPERTIES FOLDER "lib")

# imgui
add_subdirectory(./external/imgui)
set_target_properties(imgui PROPERTIES FOLDER "lib")

# tinygltf
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_HEADER_ONLY OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/tinygltf)
set_target_properties(tinygltf PROPERTIES FOLDER "lib")

# utf8
add_subdirectory(./external/utf8)
set_target_properties(utf8 PROPERTIES FOLDER "lib")

# === 添加你的主程序 ===
# 收集所有 src 下的 .cpp 文件（包括 inside, outside 和 base）
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# 添加可执行文件
add_executable(main ${SRC_FILES} "src/include/ObjectModel/ObjectModel.h" 
    "src/base/ourloader.cpp"
    "src/base/ObjectModel.cpp" 
    "src/include/base/IImGuiPanel.h" 
    "src/include/base/ModelTransformPanel.h" 
    
    "src/include/base/ImGuiController.h" 
     "src/include/OurObjLoader/OurObjLoader.h")

# 添加头文件目录
target_include_directories(main PRIVATE
    src/include
    src/include/ObjectModel
    src/include/UIsystem
    src/include/OurObjLoader
    src
    external/glad/include
    external/glfw/include
    external/glm
    external/stb
    external/imgui
    external/tinygltf
    external/utf8
)

target_link_libraries(main glad glfw glm stb imgui tinygltf utf8)

# 设置输出路径（封装函数也可）
if (MSVC OR XCODE)
    set(OUTDIR "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
elseif (EMSCRIPTEN)
    set(OUTDIR "${CMAKE_BINARY_DIR}/bin/browser")
else()
    set(OUTDIR "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
endif()
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTDIR}")
set_target_properties(main PROPERTIES FOLDER "project")
