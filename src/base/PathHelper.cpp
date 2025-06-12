#include "base/PathHelper.h"
#include <filesystem>
#include <algorithm>
#include <iostream>

// 初始化静态成员变量
std::string PathHelper::rootPath_ = "";

void PathHelper::initialize() {
    // __FILE__ 是一个宏，它会展开为当前源文件(PathHelper.cpp)的绝对路径
    std::filesystem::path sourceFilePath = __FILE__;
    
    rootPath_ = sourceFilePath.parent_path().parent_path().parent_path().string();

    // 将路径中的反斜杠 \ 替换为正斜杠 /，以保证跨平台兼容性
    std::replace(rootPath_.begin(), rootPath_.end(), '\\', '/');
}

std::string PathHelper::get(const std::string& relativePathFromRoot) {
    if (rootPath_.empty()) {
        initialize(); // 确保即使忘记调用，也能自动初始化
    }
    //std::cout << rootPath_ << std::endl;
    return rootPath_ + "/" + relativePathFromRoot;
}