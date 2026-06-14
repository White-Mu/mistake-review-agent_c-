//
// Created by White-Mu on 2026/6/14.
//
#include "nodes/node1_input.h"
#include <iostream>
namespace node1 {
    std::string run() {
        std::cout << "【节点1】输入错题信息（例：初二数学，三角形全等，公式记错，目标期末提分）：\n>>> ";
        std::string s;
        std::getline(std::cin, s);
        std::cout << "【节点1完成】接收输入\n";
        return s;
    }
}