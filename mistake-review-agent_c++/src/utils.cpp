#include "utils.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>

namespace utils {
    std::map<std::string, std::string> load_env(const std::string& path)
    {
        std::map<std::string, std::string> res;
        std::ifstream f(path);
        std::string line;
        while (std::getline(f, line)) {
            if (line.empty() || line[0] == '#') continue;
            size_t eq = line.find('=');
            if (eq == std::string::npos) continue;
            std::string k = line.substr(0, eq);
            std::string v = line.substr(eq+1);
            v.erase(0, v.find_first_not_of(" \t\"'"));
            v.erase(v.find_last_not_of(" \t\"'")+1);
            res[k] = v;
        }
        return res;
    }

    bool contains(const std::string& str, const std::string& sub) {
        return str.find(sub) != std::string::npos;
    }

    void print_separator() {
        std::cout << "\n" << std::string(50, '=') << "\n";
    }

    std::string fix_newline(std::string s)
    {
        size_t pos = 0;
        while ((pos = s.find("\\n", pos)) != std::string::npos)
        {
            s.replace(pos, 2, "\n");
            pos += 1;
        }
        return s;
    }

    std::string get_current_time()
    {
        time_t now = time(0);
        tm* t = localtime(&now);
        char buf[64];
        sprintf(buf, "%04d%02d%02d_%02d%02d%02d",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec);
        return std::string(buf);
    }

    void save_to_mistake_book(const std::string& full_report)
    {
        std::ofstream f("mistake_book.md", std::ios::app);
        if (!f.is_open())
        {
            std::cout << "⚠️  错题本保存失败\n";
            return;
        }
        std::string time_str = get_current_time();
        f << "\n---\n";
        f << "## 错题记录 " << time_str << "\n\n";
        f << full_report << "\n";
        f.close();
    }

    bool export_report_to_path(const std::string& report, const std::string& dir_path, std::string& out_filename)
    {
        std::string filename = dir_path + "错题报告_" + get_current_time() + ".md";
        std::ofstream f(filename);
        if (!f.is_open())
        {
            return false;
        }
        f << "# 错题整理与复习报告\n\n";
        f << report << "\n";
        f.close();
        out_filename = filename;
        return true;
    }

    std::string get_recent_mistakes(int count)
    {
        std::ifstream f("mistake_book.md");
        if (!f.is_open())
        {
            return "错题本还没有内容哦～";
        }

        std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();

        size_t pos = content.rfind("## 错题记录");
        int found = 0;
        std::string result;

        while (pos != std::string::npos && found < count)
        {
            size_t end = content.find("\n---", pos);
            if (end == std::string::npos) end = content.size();
            result = content.substr(pos, end - pos) + "\n\n" + result;
            found++;
            if (pos == 0) break;
            pos = content.rfind("## 错题记录", pos - 1);
        }

        if (found == 0) return "错题本还没有内容哦～";
        return "📚 最近" + std::to_string(found) + "条错题记录：\n\n" + result;
    }

    void open_mistake_book()
    {
        system("notepad.exe mistake_book.md");
    }

    std::string json_escape(const std::string& s)
    {
        std::string res;
        for (char c : s)
        {
            switch (c)
            {
                case '"':  res += "\\\""; break;
                case '\\': res += "\\\\"; break;
                case '\n': res += "\\n";  break;
                case '\r': res += "\\r";  break;
                case '\t': res += "\\t";  break;
                case '\b': res += "\\b";  break;
                case '\f': res += "\\f";  break;
                default:   res += c;      break;
            }
        }
        return res;
    }

    // 新增：字符串trim（去除首尾空格和换行）
    std::string trim(const std::string& s)
    {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    // 新增：从JSON响应中安全提取指定key的字符串值（正确处理转义引号）
    std::string extract_json_text(const std::string& resp, const std::string& key)
    {
        std::string search_key = "\"" + key + "\":\"";
        size_t p = resp.find(search_key);
        if (p == std::string::npos) return "";

        p += search_key.size(); // 跳过 key 和 ":"

        // 逐字符解析JSON字符串，正确处理转义序列
        std::string result;
        for (size_t i = p; i < resp.size(); ++i)
        {
            char c = resp[i];
            if (c == '"')
            {
                // 遇到未转义的双引号，字符串结束
                return result;
            }
            else if (c == '\\' && i + 1 < resp.size())
            {
                // 转义序列
                char next = resp[i + 1];
                switch (next)
                {
                    case '"':  result += '"';  break;
                    case '\\': result += '\\'; break;
                    case '/':  result += '/';  break;
                    case 'b':  result += '\b'; break;
                    case 'f':  result += '\f'; break;
                    case 'n':  result += '\n'; break;
                    case 'r':  result += '\r'; break;
                    case 't':  result += '\t'; break;
                    default:   result += next; break; // 包括 \uXXXX 等
                }
                i++; // 跳过下一个字符
            }
            else
            {
                result += c;
            }
        }
        return result; // 未找到结束引号，但返回已解析的内容
    }

    // 新增：尝试从API响应中提取text或content字段
    std::string extract_api_response_text(const std::string& resp)
    {
        // 先尝试 "text" 格式（通义千问）
        std::string result = extract_json_text(resp, "text");
        if (!result.empty()) return result;

        // 再尝试 "content" 格式（第三方模型如 DeepSeek）
        result = extract_json_text(resp, "content");
        return result;
    }

    // 新增：把Markdown格式转换成控制台友好的纯文本格式
    std::string format_for_console(const std::string& markdown)
    {
        std::string res;
        std::istringstream iss(markdown);
        std::string line;

        while (std::getline(iss, line))
        {
            // 1. 处理一级标题 # 标题 → ===== 标题 =====
            if (line.size() > 2 && line.substr(0, 2) == "# ") {
                std::string title = line.substr(2);
                int pad = (50 - title.size()) / 2;
                if (pad < 0) pad = 0;
                res += "\n" + std::string(pad, '=') + " " + title + " " + std::string(pad, '=') + "\n";
                continue;
            }

            // 2. 处理二级标题 ## 标题 → 【标题】
            if (line.size() > 3 && line.substr(0, 3) == "## ") {
                res += "\n【" + line.substr(3) + "】\n";
                continue;
            }

            // 3. 处理三级标题 ### 标题 → ▶ 标题
            if (line.size() > 4 && line.substr(0, 4) == "### ") {
                res += "\n▶ " + line.substr(4) + "\n";
                continue;
            }

            // 4. 处理分隔线 --- → 50个减号
            if (line == "---" || line == "***" || line == "___") {
                res += std::string(50, '-') + "\n";
                continue;
            }

            // 5. 处理无序列表 * / - → •
            if ((line.size() > 2 && line[0] == '*' && line[1] == ' ') ||
                (line.size() > 2 && line[0] == '-' && line[1] == ' ')) {
                res += "• " + line.substr(2) + "\n";
                continue;
            }

            // 6. 处理加粗 **内容** → 「内容」
            size_t pos = 0;
            while ((pos = line.find("**", pos)) != std::string::npos) {
                line.replace(pos, 2, "「");
                pos += 1;
                pos = line.find("**", pos);
                if (pos != std::string::npos) {
                    line.replace(pos, 2, "」");
                    pos += 1;
                }
            }

            // 7. 处理行内代码 `内容` → 『内容』
            pos = 0;
            while ((pos = line.find("`", pos)) != std::string::npos) {
                line.replace(pos, 1, "『");
                pos += 1;
                pos = line.find("`", pos);
                if (pos != std::string::npos) {
                    line.replace(pos, 1, "』");
                    pos += 1;
                }
            }

            // 普通行直接输出
            res += line + "\n";
        }

        return res;
    }
}