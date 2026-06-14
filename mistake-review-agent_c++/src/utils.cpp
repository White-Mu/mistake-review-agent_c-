#include "utils.h"
#include <fstream>
#include <iostream>

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
}