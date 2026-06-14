#include "nodes/node11_format.h"
#include <string>

namespace node11 {
    // 工具函数：把转义的\n换成真正的换行
    std::string fix_newline(std::string s) {
        size_t pos = 0;
        while ((pos = s.find("\\n", pos)) != std::string::npos) {
            s.replace(pos, 2, "\n");
            pos += 1;
        }
        return s;
    }

    std::string run(const std::string& diag, const std::string& fix, const std::string& plan, const std::string& material)
    {
        std::string full =
    "\n==================== 错题完整整理报告 ====================\n\n"
    "【Kimi智能诊断】\n" + fix_newline(diag) + "\n\n"
    "【针对性优化指导】\n" + fix_newline(fix) + "\n\n"
    "【3天专项复习计划】\n" + fix_newline(plan) + "\n\n"
    "【配套学习资料】\n" + fix_newline(material) + "\n"
    "\n==========================================================\n"
    "加油！坚持练习就能彻底吃透这个知识点！\n";
        return full;
    }
}