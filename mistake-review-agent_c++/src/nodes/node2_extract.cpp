#include "nodes/node2_extract.h"
#include <iostream>
namespace node2 {
    std::string run(const std::string& raw) {
        std::cout << "【节点2完成】信息提取\n";
        return "原始错题：" + raw + "\n提取：年级/科目/题型/知识点/错因/学习目标";
    }
}