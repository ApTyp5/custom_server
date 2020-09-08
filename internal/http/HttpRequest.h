//
// Created by pycnick on 03.09.2020.
//

#ifndef WEBSERVER_HTTP_H
#define WEBSERVER_HTTP_H

#include <iostream>
#include <sstream>
#include <vector>

class HttpRequest {
public:
    explicit HttpRequest(std::string data);
    ~HttpRequest() = default;

    std::string getMethod();
    std::string getPath();

private:
    std::string method;
    std::string path;
};


#endif //WEBSERVER_HTTP_H
