#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <map>

namespace utils {
    std::string fix_newline(std::string s);
    std::map<std::string, std::string> load_env(const std::string& path);
    bool contains(const std::string& str, const std::string& sub);
    void print_separator();
    std::string get_current_time();
    void save_to_mistake_book(const std::string& full_report);
    bool export_report_to_path(const std::string& report, const std::string& dir_path, std::string& out_filename);
    std::string get_recent_mistakes(int count = 3);
    void open_mistake_book();
    std::string json_escape(const std::string& s);
    std::string format_for_console(const std::string& markdown); // 新增：Markdown转控制台友好格式
    std::string trim(const std::string& s); // 新增：字符串trim
    std::string extract_json_text(const std::string& resp, const std::string& key); // 新增：安全提取JSON字符串（处理转义引号）
    std::string extract_api_response_text(const std::string& resp); // 新增：尝试从API响应中提取text/content
}

#endif