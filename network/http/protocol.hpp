#pragma once 
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Util.hpp"

const std::string sep = "\r\n";
const std::string default_root = "./wwwroot";
const std::string home_page = "index.html";

class HttpRequest
{
public:
    HttpRequest() = default;
    ~HttpRequest() = default;

    void parse()
    {
        // 1. 从_inBuffer中拿到第一行，分隔符\r\n
        std::string line = Util::getOneLine(_inBuffer, sep);
        if (line.empty()) return;

        // 2. 从请求行中提取三个字段
        std::cout << "line" << line << std::endl;

        std::stringstream ss(line);
        ss >> _method >> _url >> _httpVersion;

        // 3. 添加web默认路径
        _path = default_root;  // ./wwwroot
        _path += _url;  // ./wwwroot/a/b/c.html
        if (_path[_path.size()-1] == '/') _path += home_page;
    }

public:
    std::string _inBuffer;

    // std::string _reqLine;
    // std::vector<std::string> _reqHeader;
    // std::string _body;
    
    std::string _method;
    std::string _url;
    std::string _httpVersion;
    std::string _path;
};

class HttpResponse
{
public:

public:
    std::string _outBuffer;

};