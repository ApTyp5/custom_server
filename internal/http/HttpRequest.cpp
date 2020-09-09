//
// Created by pycnick on 03.09.2020.
//

#include "HttpRequest.h"

std::string removeParams(std::string str) {
    std::string::size_type pos = str.find('?');
    if (pos < 0) {
        return str;
    }

    return str.substr(0, pos);
}

std::string urlDecode(std::string str){
    std::string ret;
    char ch;
    int i, ii, len = str.length();

    for (i=0; i < len; i++){
        if(str[i] != '%'){
            if(str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }else{
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}

std::vector<std::string> split(const char *phrase, std::string delimiter){
    std::vector<std::string> list;
    std::string s = std::string(phrase);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}

HttpRequest::HttpRequest(std::string data) {
    auto vec = split(data.c_str(), " ");

    if (vec.size() < 2) {
        path = "None";
        method = "None";

        return;
    }

    method = vec[0];
    path = vec[1];

    path = urlDecode(path);
    path = removeParams(path);
}

std::string HttpRequest::getMethod() {
    return method;
}

std::string HttpRequest::getPath() {
    return path;
}
