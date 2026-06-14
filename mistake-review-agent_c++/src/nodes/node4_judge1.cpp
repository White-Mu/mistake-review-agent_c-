#include "nodes/node4_judge1.h"
#include "utils.h"
#include <iostream>
namespace node4 {
    std::string run(const std::string& res) {
        if (utils::contains(res, "基础概念漏洞")) {
            std::cout << "【节点4】判定：基础漏洞→分支A\n";
            return "branch_a";
        }
        std::cout << "【节点4】判定：能力不足→分支B\n";
        return "branch_b";
    }
}