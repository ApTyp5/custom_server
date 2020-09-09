//
// Created by pycnick on 03.09.2020.
//

#ifndef CUSTOM_SERVER_SERVER_H
#define CUSTOM_SERVER_SERVER_H

#define READ_BUFF_SIZE 128
#define WRITE_BUFF_SIZE ((READ_BUFF_SIZE)*8)
#define EVENTS_MAX_COUNT 32

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <cstring>
#include <netdb.h>
#include <array>
#include <utility>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"
#include "../tools/FileManager.h"
#include <fstream>

#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <csignal>

#include <filesystem>

class Server {
public:
    explicit Server(std::string port, int threadsCount, std::string docRoot);
    ~Server();

    int runServer();

private:
    int create_and_bind(std::string const &port);

    int make_socket_nonblocking(int socketfd);

    int accept_connection(int socketfd, struct epoll_event &event, int epollfd);

    void work(std::string root);

    std::string read_data(int fd);
    void write_data(int fd, std::string buf);

    int initEpoll(int socket_fd);

    [[noreturn]] void runPoller();

    int maxThreads;
    std::string root;
    std::string port;
    struct epoll_event event{};

    int socketFd;
    int epollFd;

    bool done = false;

    std::queue<int> q;
    std::mutex *mu = new(std::mutex);
    std::vector<std::thread> workers;
};


#endif //CUSTOM_SERVER_SERVER_H
