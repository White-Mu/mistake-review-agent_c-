#include "nodes/node3_kimi_analysis.h"
#include "http_client.h"
#include "utils.h"
#include <iostream>

namespace node3 {
    std::string fix_newline(std::string s) {
        size_t pos = 0;
        while ((pos = s.find("\\n", pos)) != std::string::npos) {
            s.replace(pos, 2, "\n");
            pos += 1;
        }
        return s;
    }

    std::string run(const std::string& info, const std::map<std::string, std::string>& env)
    {
        std::string token = env.at("QWEN_API_KEY");
        const std::string url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation";
        std::string prompt = "你是中学错题诊断老师，从三点分析：知识掌握、解题思路、计算习惯。必须输出：漏洞类型（基础概念漏洞/解题能力不足）、难度（简单/中等/困难）。错题信息：" + info;

        std::string body = "{\"model\":\"moonshot-v1-8k\",\"input\":{\"prompt\":\"" + prompt + "\"},\"parameters\":{\"temperature\":0.3}}";
        std::map<std::string, std::string> header = {{"Authorization","Bearer "+token},{"Content-Type","application/json"}};

        std::cout << "【节点3】Kimi大模型分析中...\n";
        std::string resp = HttpClient::post_json(url, body, header);

        size_t pos = resp.find("\"text\":\"");
        std::string ret = "诊断完成";
        if (pos != std::string::npos) {
            pos += 8;
            size_t end = resp.find("\"", pos);
            if (end != std::string::npos) {
                ret = resp.substr(pos, end - pos);
            }
        }

        ret = fix_newline(ret);
        std::cout << "【节点3完成】诊断结果：\n" << ret << "\n";
        return ret;
    }
}