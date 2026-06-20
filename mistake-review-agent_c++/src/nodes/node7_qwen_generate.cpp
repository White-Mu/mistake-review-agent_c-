#include "nodes/node7_qwen_generate.h"
#include "http_client.h"
#include "utils.h"
#include <iostream>

namespace node7
{
    std::string run(const std::string& branch_content, const std::map<std::string, std::string>& env)
    {
        std::string token = env.at("QWEN_API_KEY");
        const std::string url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation";

        std::string prompt = "根据下面的错题分析和解题指导，为中学生制定一份详细的7天专项复习计划，要求：\n"
        "1. 每天的复习时间控制在20-30分钟，符合中学生作业多的情况\n"
        "2. 7天梯度安排：第1-2天基础巩固，第3-5天专项练习，第6天错题回顾，第7天综合检测\n"
        "3. 每天都要写清楚：当天目标、具体任务、练习数量、完成标准\n"
        "4. 内容要具体可执行，不要说空话，比如不说「做练习题」，要说「做5道同类型基础题」\n"
        "5. 最后加一段复习注意事项，说明复习时的重点和容易踩的坑\n"
        "内容依据：" + branch_content;

        std::string escaped_prompt = utils::json_escape(prompt);
        // ✅ 在这里修改【复习计划模型】名称
        std::string body = R"({"model":"qwen-turbo","input":{"prompt":")" + escaped_prompt + R"("},"parameters":{"temperature":0.7}})";
        std::map<std::string, std::string> hd = {{"Authorization", "Bearer " + token}, {"Content-Type", "application/json"}};

        std::cout << "【节点7】通义千问生成7天复习计划中...\n";
        std::string resp = HttpClient::post_json(url, body, hd);

        std::string result = utils::extract_api_response_text(resp);
        if (result.empty()) result = "复习计划生成失败";
        result = utils::fix_newline(result);

        std::cout << "【节点7完成】7天复习方案生成完毕\n";
        return result;
    }
}