#include "nodes/node5_branch_basic.h"
#include "http_client.h"
#include "utils.h"
#include <iostream>

namespace node5
{
    std::string run(const std::string& diag_result, const std::map<std::string, std::string>& env)
    {
        std::string token = env.at("QWEN_API_KEY");
        const std::string url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation";

        std::string prompt = "根据下面的错题诊断，生成详细的基础知识点补全内容，包含以下4个部分：\n"
        "1. 核心概念讲解：用中学生能听懂的话，把这道题涉及的核心知识点讲明白\n"
        "2. 相关公式/定理：列出所有用到的公式、定理、定义，每个配简单说明\n"
        "3. 易错点提醒：列出这个知识点最容易记错、搞混的3个地方\n"
        "4. 记忆小技巧：给一个好记的口诀或者方法，帮学生记住这个知识点\n"
        "诊断内容：" + diag_result;

        std::string escaped_prompt = utils::json_escape(prompt);
        // ✅ 在这里修改【基础补全模型】名称
        std::string body = R"({"model":"qwen-turbo","input":{"prompt":")" + escaped_prompt + R"("},"parameters":{"temperature":0.4}})";
        std::map<std::string, std::string> hd = {{"Authorization", "Bearer " + token}, {"Content-Type", "application/json"}};

        std::cout << "【节点5】生成基础知识点补全中...\n";
        std::string resp = HttpClient::post_json(url, body, hd);

        std::string result = utils::extract_api_response_text(resp);
        if (result.empty()) result = "基础知识点补全生成失败";
        result = utils::fix_newline(result);

        std::cout << "【节点5完成】基础知识点补全完毕\n";
        return result;
    }
}