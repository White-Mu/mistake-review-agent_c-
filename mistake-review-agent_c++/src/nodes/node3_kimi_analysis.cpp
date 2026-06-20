#include "nodes/node3_kimi_analysis.h"
#include "http_client.h"
#include "utils.h"
#include <iostream>

namespace node3
{
    std::string run(const std::string& input, const std::map<std::string, std::string>& env)
    {
        std::string token = env.at("QWEN_API_KEY");
        const std::string url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation";

        std::string prompt = "你是资深中学错题诊断老师，请对下面这道错题做深度分析，输出格式分以下6点，每点都要详细：\n"
        "1. 错误根本原因：详细说明学生为什么会做错，是概念没懂还是思路错了\n"
        "2. 涉及知识点：列出这道题考察的所有知识点，每个知识点简单解释\n"
        "3. 漏洞类型：判断是「基础概念漏洞」还是「解题能力不足」，只输出这八个字其中一个\n"
        "4. 题目难度：简单/中等/困难，三选一\n"
        "5. 同类题易错点：说明这类题学生最容易踩的坑有哪些\n"
        "6. 掌握程度评估：用1-10分给学生当前对这个知识点的掌握程度打分，并说明理由\n"
        "错题信息：" + input;

        std::string escaped_prompt = utils::json_escape(prompt);
        // ✅ 这里换模型名就行，通义/DeepSeek/智谱都能用
        std::string body = R"({"model":"deepseek-v3","input":{"prompt":")" + escaped_prompt + R"("},"parameters":{"temperature":0.2}})";
        std::map<std::string, std::string> hd = {{"Authorization", "Bearer " + token}, {"Content-Type", "application/json"}};

        std::cout << "【节点3】DeepSeek大模型深度诊断中...\n";
        std::string resp = HttpClient::post_json(url, body, hd);

        std::string result = utils::extract_api_response_text(resp);

        result = utils::fix_newline(result);

        if (result.size() < 10) {
            std::cout << "❌ 节点3失败：诊断模型调用失败\n";
            std::cout << "[调试] 返回内容：" << resp << "\n";
            return "";
        }

        std::cout << "【节点3完成】深度诊断完毕\n";
        return result;
    }
}