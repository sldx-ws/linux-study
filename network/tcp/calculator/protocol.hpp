#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>   // recv
#include <sys/socket.h>  // recv
#include <jsoncpp/json/json.h>

#define SEP " "
#define SEP_LEN strlen(SEP)
#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

enum 
{
    OK = 0,
    DIV_ZERO,
    MOD_ZERO,
    OP_ERROR
};

// "x op y" -> "content_len"\r\n"x op y"\r\n
// "exitCode result" -> "content_len"\r\n"exitCode result"\r\n
const std::string enLength(const std::string& text)
{
    std::string send_string = std::to_string(text.size());
    send_string += LINE_SEP;
    send_string += text;
    send_string += LINE_SEP;

    return send_string;
}

// "content_len"\r\n"exitCode result"\r\n
bool deLength(const std::string& package, std::string* text)
{
    auto pos = package.find(LINE_SEP);
    if (pos == std::string::npos) return false;

    std::string text_len_string = package.substr(0, pos);
    int text_len = std::stoi(text_len_string);
    *text = package.substr(pos + LINE_SEP_LEN, text_len);

    return true;
}

// "content_len"
// "content_len"\r\n"x op 
// "content_len"\r\n"x op y"\r\n
// "content_len"\r\n"x op y"\r\n"content_len"\r\n"x op y"\r\n
bool recvPackage(int sock, std::string& inBuffer, std::string* text)
{
    char buffer[1024];
    while (true)
    {
        ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            inBuffer += buffer;

            // 分析处理
            auto pos = inBuffer.find(LINE_SEP);
            if (pos == std::string::npos) continue;

            std::string text_len_string = inBuffer.substr(0, pos);
            int text_len = std::stoi(text_len_string);

            // text_len_string + "\r\n" + text + "\r\n" <= inBuffer.size(); 至少有一个完整报文
            int total_len = text_len_string.size() + 2 * LINE_SEP_LEN + text_len;

            std::cout << "处理前#inBuffer:\n" << inBuffer << std::endl;
            if (inBuffer.size() < total_len) 
            {
                std::cout << "输入的消息未严格遵守协议, 正在等待后续内容, continue" << std::endl;
                continue;
            }
            
            // 至少有一个完整报文
            *text = inBuffer.substr(0, total_len);
            inBuffer.erase(0, total_len);

            std::cout << "处理后#inBuffer:\n " << inBuffer << std::endl;
            
            break;
        }
        else
        {
            return false;
        }
    }

    return true;
}

// bool recvPackageAll(int sock, std::vector<std::string>* out)
// {
//     std::string line;
//     while (recvRequest(sock, &line)) out->push_back(line);
// }

class Request
{
public: 
    Request(int x = 0, int y = 0, char op = 0)
        : _x(x)
        , _y(y)
        , _op(op)
    {}

    bool serialize(std::string* out)
    {
#ifdef MYSELF
        *out = "";

        // 结构化 -> "_x op _y"
        std::string _x_string = std::to_string(_x);
        std::string _y_string = std::to_string(_y);

        *out = _x_string + SEP + _op + SEP + _y_string;
#else
        Json::Value root;
        root["first"] = _x;
        root["second"] = _y;
        root["oper"] = _op;

        Json::FastWriter writer;
        // Json::StyledWriter writer;

        *out = writer.write(root);
#endif
        return true;
    }

    bool deserialize(const std::string& in)
    {
#ifdef MYSELF
        // "xxx op yyyy" -> 结构化
        auto left_sep = in.find(SEP);
        auto right_sep = in.rfind(SEP);
        if (left_sep == std::string::npos \
            || right_sep == std::string::npos)
            return false;       
        if (left_sep == right_sep) return false;
        if (right_sep - (left_sep + SEP_LEN) != 1) return false;

        std::string _x_string = in.substr(0, left_sep);
        std::string _y_string = in.substr(0, right_sep + SEP_LEN);
        if (_y_string.empty() || _x_string.empty()) return false;
        _x = std::stoi(_x_string);
        _y = std::stoi(_x_string);
        _op = in[left_sep + SEP_LEN];
#else
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);

        _x = root["first"].asInt();
        _y = root["second"].asInt();
        _op = root["oper"].asInt();
#endif
        return true;
    }

public:
    int _x;
    int _y;
    char _op;
};

class Response
{
public:
    Response(int exitCode = 0, int result = 0)
        : _exitCode(exitCode)
        , _result(result)
    {}

    bool serialize(std::string* out)
    {
#ifdef MYSELF
        std::string ec_string = std::to_string(_exitCode);
        std::string res_string = std::to_string(_result);

        *out = ec_string + SEP + res_string;
#else
        Json::Value root;
        root["exitCode"] = _exitCode;
        root["result"] = _result;

        Json::FastWriter writer;
        *out = writer.write(root);
#endif
        return true;
    }

    bool deserialize(const std::string& in)
    {
#ifdef MYSELF
        // "exitCode result"
        auto mid = in.find(SEP);
        if (mid == std::string::npos) return false;

        std::string ec_string = in.substr(0, mid);
        std::string res_string = in.substr(mid + SEP_LEN);
        if (ec_string.empty() || res_string.empty()) return false;

        _exitCode = std::stoi(ec_string);
        _result = std::stoi(res_string);
#else
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);

        _exitCode = root["exitCode"].asInt();
        _result = root["result"].asInt();
#endif
        return true;
    }

public:
    int _exitCode;
    int _result;
};