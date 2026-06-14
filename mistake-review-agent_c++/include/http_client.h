#pragma once
#include <string>
#include <map>

class HttpClient {
public:
    static std::string post_json(const std::string& url, const std::string& json_body, const std::map<std::string, std::string>& headers);
};
