//
// Created by pycnick on 05.09.2020.
//

#ifndef WEBSERVER_HTTPRESPONSE_H
#define WEBSERVER_HTTPRESPONSE_H

#include <fstream>
#include <map>
#include <chrono>
#include <ctime>

enum Codes {
    STATUS_OK = 1,
    STATUS_NOT_FOUND,
    STATUS_FORBIDDEN,
    STATUS_NOT_ALLOWED
};

class HttpResponse {
public:
    HttpResponse() = default;
    explicit HttpResponse(int status);

    void setBody(const std::string& file);

    void setHeader(const std::string& key, const std::string& value);

    std::string toString();

    std::string getHeaders();

private:
    int status;
    std::string headers;;
    std::string body;
    std::string response;
    std::map<int, std::string> codes = {
            {STATUS_OK,          "200 OK"},
            {STATUS_NOT_FOUND,   "404 NOT_FOUND"},
            {STATUS_FORBIDDEN,   "403 FORBIDDEN"},
            {STATUS_NOT_ALLOWED, "405 METHOD_NOT_ALLOWED"},
    };

};


#endif //WEBSERVER_HTTPRESPONSE_H
