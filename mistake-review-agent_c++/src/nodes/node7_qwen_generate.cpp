#include "nodes/node7_qwen_generate.h"
#include "http_client.h"
#include <iostream>

namespace node7 {
    std::string fix_newline(std::string s) {
        size_t pos = 0;
        while ((pos = s.find("\\n", pos)) != std::string::npos) {
            s.replace(pos, 2, "\n");
            pos += 1;
        }
        return s;
    }

    std::string run(const std::string& data, const std::map<std::string, std::string>& env)
    {
        std::string token = env.at("QWEN_API_KEY");
        const std::string url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation";
        std::string prompt = "根据下面错题诊断，生成3天中学生专项复习计划，标注每日任务时长：" + data;

        std::string body = "{\"model\":\"qwen-turbo\",\"input\":{\"prompt\":\"" + prompt + "\"},\"parameters\":{\"temperature\":0.7}}";
        std::map<std::string, std::string> header = {{"Authorization","Bearer "+token},{"Content-Type","application/json"}};

        std::cout << "【节点7】通义千问生成复习计划中...\n";
        std::string resp = HttpClient::post_json(url, body, header);

        size_t pos = resp.find("\"text\":\"");
        std::string ret = "计划生成完成";
        if (pos != std::string::npos) {
            pos += 8;
            size_t end = resp.find("\"", pos);
            if (end != std::string::npos) {
                ret = resp.substr(pos, end - pos);
            }
        }

        ret = fix_newline(ret);
        std::cout << "【节点7完成】复习方案生成完毕\n";
        return ret;
    }
}