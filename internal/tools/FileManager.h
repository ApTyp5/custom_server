//
// Created by pycnick on 05.09.2020.
//

#ifndef WEBSERVER_FILEMANAGER_H
#define WEBSERVER_FILEMANAGER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <string_view>

class FileManager {
public:
    explicit FileManager(std::string path);
    ~FileManager();

    bool isExists();

    bool isForbidden();

    std::string getFileBuf();

    std::string getContentType();

private:
    bool exists;
    bool forbidden;
    std::string path;
    std::fstream file;
};


#endif //WEBSERVER_FILEMANAGER_H
