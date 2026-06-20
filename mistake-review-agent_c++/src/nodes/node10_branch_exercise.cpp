#include "nodes/node10_branch_exercise.h"
#include "http_client.h"
#include "utils.h"
#include <iostream>

namespace node10
{
    std::string run(const std::string& plan_content, const std::map<std::string, std::string>& env)
    {
        std::string token = env.at("QWEN_API_KEY");
        const std::string url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation";

        std::string prompt = "根据下面的复习计划，出3道同难度同知识点的变式练习题，要求：\n"
        "1. 每道题都要有题目、参考答案、详细解析三个部分\n"
        "2. 3道题难度梯度：第1道基础，第2道中等，第3道稍难\n"
        "3. 每道题的解析都要讲清楚解题思路和易错点\n"
        "复习内容：" + plan_content;

        std::string escaped_prompt = utils::json_escape(prompt);
        // ✅ 在这里修改【变式题模型】名称
        std::string body = R"({"model":"qwen-turbo","input":{"prompt":")" + escaped_prompt + R"("},"parameters":{"temperature":0.6}})";
        std::map<std::string, std::string> hd = {{"Authorization", "Bearer " + token}, {"Content-Type", "application/json"}};

        std::cout << "【节点10】生成3道变式练习题中...\n";
        std::string resp = HttpClient::post_json(url, body, hd);

        std::string result = utils::extract_api_response_text(resp);
        if (result.empty()) result = "练习题生成失败";
        result = utils::fix_newline(result);

        std::cout << "【节点10完成】练习题生成完毕\n";
        return result;
    }
}