//
// Created by Lenovo on 25-6-8.
//

#ifndef PATHHELPER_H
#define PATHHELPER_H
#pragma once
#include <string>

class PathHelper {
public:
    // 初始化，用于在程序启动时计算根目录
    static void initialize();

    // 获取资源的绝对路径
    static std::string get(const std::string& relativePathFromRoot);

private:
    static std::string rootPath_;
};
#endif //PATHHELPER_H
