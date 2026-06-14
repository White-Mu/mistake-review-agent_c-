#include "utils.h"
#include "nodes/node1_input.h"
#include "nodes/node2_extract.h"
#include "nodes/node3_kimi_analysis.h"
#include "nodes/node4_judge1.h"
#include "nodes/node5_branch_basic.h"
#include "nodes/node6_branch_logic.h"
#include "nodes/node7_qwen_generate.h"
#include "nodes/node8_judge2.h"
#include "nodes/node9_branch_explain.h"
#include "nodes/node10_branch_exercise.h"
#include "nodes/node11_format.h"
#include "nodes/node12_output.h"
#include <map>
#include <iostream>

int main()
{
    // 解决Windows中文乱码
    system("chcp 65001 > nul");

    utils::print_separator();
    std::cout << "🚀 12节点双大模型错题整理智能体启动\n";
    utils::print_separator();

    // 加载密钥配置 + 异常处理
    std::map<std::string, std::string> env = utils::load_env(".env");
    if (env.find("QWEN_API_KEY") == env.end()) {
        std::cout << "❌ 错误：找不到API密钥配置！\n";
        std::cout << "请在项目根目录创建 .env 文件，内容为：\n";
        std::cout << "QWEN_API_KEY = \"你的阿里云百炼API密钥\"\n";
        return 1;
    }

    // 12节点顺序执行
    std::string n1 = node1::run();
    std::string n2 = node2::run(n1);
    std::string n3 = node3::run(n2, env);
    std::string n4 = node4::run(n3);

    std::string branch1 = (n4 == "branch_a") ? node5::run(n3) : node6::run(n3);

    std::string n7 = node7::run(branch1, env);
    std::string n8 = node8::run(n7);

    std::string branch2 = (n8 == "need_explain") ? node9::run(n7) : node10::run(n7);

    std::string n11 = node11::run(n3, branch1, n7, branch2);
    node12::run(n11);

    std::cout << "\n✅ 全部12节点执行完成！\n";
    return 0;
}