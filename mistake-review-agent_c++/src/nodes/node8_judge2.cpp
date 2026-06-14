#include "nodes/node8_judge2.h"
#include "utils.h"
#include <iostream>
namespace node8 {
    std::string run(const std::string& plan) {
        if (utils::contains(plan, "中等") || utils::contains(plan, "困难")) {
            std::cout << "【节点8】判定：需要讲解→分支A\n";
            return "need_explain";
        }
        std::cout << "【节点8】判定：需要变式题→分支B\n";
        return "need_exercise";
    }
}
