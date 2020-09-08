//
// Created by pycnick on 05.09.2020.
//

#include "FileManager.h"
#include <filesystem>
#include <iostream>

FileManager::FileManager(std::string path) : path(path) {
    if (std::string_view(path).ends_with("/")) {
        path = path + "index.html";
    }

    exists = std::filesystem::exists(path);
    forbidden = false;

    if (exists) {
        file = std::fstream(path);

        if (!file.is_open()) {
            forbidden = true;
        }

        return;
    }
    forbidden = true;
}

bool FileManager::isExists() {
    return exists;
}

bool FileManager::isForbidden() {
    return forbidden;
}

std::string FileManager::getFileBuf() {
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

std::string FileManager::getContentType() {
    if (path.ends_with("html") || path.ends_with("htm")) {
        return "text/html";
    }
    else if (path.ends_with("css")) {
        return "text/css";
    }
    else if (path.ends_with("gif")) {
        return "image/gif";
    }
    else if (path.ends_with("png")) {
        return "image/png";
    }
    else if (path.ends_with("jpg") || path.ends_with("jpe") || path.ends_with("jpeg")) {
        return "image/jpeg";
    }
    else if (path.ends_with("tiff") || path.ends_with("tif")) {
        return "image/tiff";
    }
    else if (path.ends_with("bmp")) {
        return "image/bmp";
    }
    else if (path.ends_with("js")) {
        return "application/javascript";
    }
    else if (path.ends_with("swf")) {
        return "application/x-shockwave-flash";
    }
    return "text/plain";
}

FileManager::~FileManager() {
    file.close();
}
