# 定义一个接口库。接口库不包含源文件，只用于传递属性（例如包含路径）。
add_library(utf8 INTERFACE)

# 设置接口库的包含路径。
# PUBLIC/INTERFACE 意味着所有链接到 utf8 的目标都会自动获得这些包含路径。
target_include_directories(utf8 INTERFACE
        # 这是库的主头文件 utf8.h 所在的目录
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>

        # 这是 utf8 子文件夹（external/utf8/utf8/）中的头文件所在的目录
        # 如果这些头文件也需要被外部直接包含，就添加这一行
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utf8>
        $<INSTALL_INTERFACE:include/utf8>
)

# 设置 FOLDER 属性，用于 IDE 中的分组
set_target_properties(utf8 PROPERTIES FOLDER "lib")

# （可选）安装规则：将头文件安装到指定位置
install(TARGETS utf8 EXPORT utf8_targets DESTINATION lib) # 尽管是接口库，但导出规则仍有用

# 安装具体的头文件
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/utf8.h" DESTINATION include)
# 安装整个子文件夹的头文件
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/utf8/"
        DESTINATION include/utf8
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" # 只安装 .h 和 .hpp 文件
)