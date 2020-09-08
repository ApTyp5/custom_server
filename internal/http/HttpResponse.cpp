//
// Created by pycnick on 05.09.2020.
//

#include "HttpResponse.h"

HttpResponse::HttpResponse(int status) : status(status) {
    setHeader("HTTP/1.1", codes[status]);
    setHeader("Server:", "custom_server");
}

void HttpResponse::setBody(const std::string& file) {
    body = file;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    headers += key + " " + value + "\r\n";
}

std::string HttpResponse::toString() {
    return headers + "\r\n" + body;
}

std::string HttpResponse::getHeaders() {
    return headers;
}
