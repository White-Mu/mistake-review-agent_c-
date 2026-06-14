#include "http_client.h"
#include <windows.h>
#include <winhttp.h>
#include <iostream>

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "ws2_32.lib")

std::string HttpClient::post_json(const std::string& url, const std::string& json_body, const std::map<std::string, std::string>& headers)
{
    std::string resp;
    HINTERNET hSession = WinHttpOpen(L"C++Agent/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
    if (!hSession) return "会话创建失败";

    URL_COMPONENTS uc = {0};
    wchar_t host[256] = {0}, path[1024] = {0};
    uc.dwStructSize = sizeof(URL_COMPONENTS);
    uc.lpszHostName = host; uc.dwHostNameLength = ARRAYSIZE(host);
    uc.lpszUrlPath = path; uc.dwUrlPathLength = ARRAYSIZE(path);

    std::wstring wurl(url.begin(), url.end());
    if (!WinHttpCrackUrl(wurl.c_str(), wurl.size(), 0, &uc)) {
        WinHttpCloseHandle(hSession);
        return "URL解析失败";
    }

    HINTERNET hConn = WinHttpConnect(hSession, host, uc.nPort, 0);
    if (!hConn) { WinHttpCloseHandle(hSession); return "连接失败"; }

    DWORD sec = (uc.nPort == 443) ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hReq = WinHttpOpenRequest(hConn, L"POST", path, nullptr, nullptr, nullptr, sec);
    if (!hReq) {
        WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSession);
        return "请求创建失败";
    }

    for (auto& p : headers) {
        std::wstring line(p.first.begin(), p.first.end());
        line += L": " + std::wstring(p.second.begin(), p.second.end());
        WinHttpAddRequestHeaders(hReq, line.c_str(), line.size(), WINHTTP_ADDREQ_FLAG_ADD);
    }

    if (!WinHttpSendRequest(hReq, nullptr, 0, (LPVOID)json_body.c_str(), json_body.size(), json_body.size(), 0)) {
        WinHttpCloseHandle(hReq); WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSession);
        return "发送失败";
    }

    if (!WinHttpReceiveResponse(hReq, nullptr)) {
        WinHttpCloseHandle(hReq); WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSession);
        return "接收响应失败";
    }

    char buf[4096] = {0};
    DWORD read = 0;
    do {
        ZeroMemory(buf, sizeof(buf));
        if (WinHttpReadData(hReq, buf, sizeof(buf)-1, &read))
            resp.append(buf, read);
    } while (read > 0);

    WinHttpCloseHandle(hReq);
    WinHttpCloseHandle(hConn);
    WinHttpCloseHandle(hSession);
    return resp;
}