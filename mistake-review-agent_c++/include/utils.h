#pragma once
#include <string>
#include <map>

namespace utils {
    std::map<std::string, std::string> load_env(const std::string& path);
    bool contains(const std::string& str, const std::string& substr);
    void print_separator();
}