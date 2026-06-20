#include "nodes/node6_branch_logic.h"
#include "http_client.h"
#include "utils.h"
#include <iostream>

namespace node6
{
    std::string run(const std::string& diag_result, const std::map<std::string, std::string>& env)
    {
        std::string token = env.at("QWEN_API_KEY");
        const std::string url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation";

        std::string prompt = "根据下面的错题诊断，生成详细的解题逻辑优化指南，包含以下5个部分，每部分都要具体：\n"
        "1. 破题技巧：拿到这道题第一步应该怎么想，怎么找突破口\n"
        "2. 标准解题步骤：一步一步写出完整解题过程，每一步都标注为什么这么做\n"
        "3. 错误思路对比：说明学生刚才的错误思路错在哪里，和正确思路的区别\n"
        "4. 同类题解题套路：总结这类题的通用解题方法，以后遇到同类题直接套用\n"
        "5. 思维误区提醒：列出做这类题最容易犯的3个思维错误，提醒注意\n"
        "诊断内容：" + diag_result;

        std::string escaped_prompt = utils::json_escape(prompt);
        // ✅ 在这里修改【解题优化模型】名称
        std::string body = R"({"model":"qwen-turbo","input":{"prompt":")" + escaped_prompt + R"("},"parameters":{"temperature":0.5}})";
        std::map<std::string, std::string> hd = {{"Authorization", "Bearer " + token}, {"Content-Type", "application/json"}};

        std::cout << "【节点6】生成详细解题指南中...\n";
        std::string resp = HttpClient::post_json(url, body, hd);

        std::string result = utils::extract_api_response_text(resp);
        if (result.empty()) result = "解题指南生成失败";
        result = utils::fix_newline(result);

        std::cout << "【节点6完成】解题指南生成完毕\n";
        return result;
    }
}