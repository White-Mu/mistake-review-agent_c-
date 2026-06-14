#include "nodes/node12_output.h"
#include "utils.h"
#include <iostream>
namespace node12 {
    void run(const std::string& text) {
        utils::print_separator();
        std::cout << text;
        utils::print_separator();
        std::cout << "【节点12】全部流程输出完毕\n";
    }
}