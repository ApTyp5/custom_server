#include "Server.h"

Server::Server(std::string port, int threadsCount, std::string docRoot) : port(std::move(port)), maxThreads(threadsCount), root(std::move(docRoot)) {}

int Server::runServer() {
    socketFd = create_and_bind(port);
    if (socketFd == -1) {
        return 1;
    }

    if (!make_socket_nonblocking(socketFd)) {
        return 1;
    }

    if (listen(socketFd, SOMAXCONN) == -1) {
        std::cerr << "[E] listen failed\n";
        return 1;
    }

    if (initEpoll(socketFd)) {
        return 1;
    }

    for (int i = 0; i < maxThreads; i++) {
        workers.emplace_back([this](){
            this->work(root);
        });
    }

    runPoller();

    return 0;
}

int Server::initEpoll(int socket_fd) {
    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        std::cerr << "[E] epoll_create1 failed\n";
        return 1;
    }

    event.data.fd = socket_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
        std::cerr << "[E] epoll_ctl failed\n";
        return 1;
    }

    return 0;
}


void Server::runPoller() {
    std::array<struct epoll_event, EVENTS_MAX_COUNT> events;

    while (true) {
        auto n = epoll_wait(epollFd, events.data(), EVENTS_MAX_COUNT, -1);
        for (int i = 0; i < n; ++i) {
            if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP || !(events[i].events & EPOLLIN)) {
                close(events[i].data.fd);
            } else if (socketFd == events[i].data.fd) {
                while (accept_connection(socketFd, event, epollFd)) {}
            } else {
                auto fd = events[i].data.fd;
                mu->lock();
                q.push(fd);
                mu->unlock();
            }
        }
    }
}

int Server::create_and_bind(std::string const &port) {
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_flags = AI_PASSIVE; /* All interfaces */

    struct addrinfo *result;
    int sockt = getaddrinfo(nullptr, port.c_str(), &hints, &result);
    if (sockt != 0) {
        std::cerr << "[E] getaddrinfo failed\n";
        return -1;
    }

    struct addrinfo *rp;
    int socketfd;
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        socketfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socketfd == -1) {
            continue;
        }

        sockt = bind(socketfd, rp->ai_addr, rp->ai_addrlen);
        if (sockt == 0) {
            break;
        }

        close(socketfd);
    }

    if (rp == nullptr) {
        std::cerr << "[E] bind failed\n";
        return -1;
    }

    freeaddrinfo(result);

    return socketfd;
}

int Server::make_socket_nonblocking(int socketfd) {
    int flags = fcntl(socketfd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "[E] fcntl failed (F_GETFL)\n";
        return false;
    }

    flags |= O_NONBLOCK;
    int s = fcntl(socketfd, F_SETFL, flags);
    if (s == -1) {
        std::cerr << "[E] fcntl failed (F_SETFL)\n";
        return false;
    }

    return true;
}

int Server::accept_connection(int socketfd, struct epoll_event &event, int epollfd) {
    struct sockaddr in_addr;
    socklen_t in_len = sizeof(in_addr);
    int infd = accept(socketfd, &in_addr, &in_len);
    if (infd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // Done processing incoming connections
        {
            return false;
        } else {
            std::cerr << "[E] accept failed\n";
            return false;
        }
    }

    std::string hbuf(NI_MAXHOST, '\0');
    std::string sbuf(NI_MAXSERV, '\0');
    if (getnameinfo(&in_addr, in_len,
                    const_cast<char *>(hbuf.data()), hbuf.size(),
                    const_cast<char *>(sbuf.data()), sbuf.size(),
                    NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        std::cout << "[I] Accepted connection on descriptor " << infd << "(host=" << hbuf << ", port=" << sbuf
                  << ")" << "\n";
    }

    if (!make_socket_nonblocking(infd)) {
        std::cerr << "[E] make_socket_nonblocking failed\n";
        return false;
    }

    event.data.fd = infd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, infd, &event) == -1) {
        std::cerr << "[E] epoll_ctl failed\n";
        return false;
    }

    return true;
}

std::string Server::read_data(int fd) {
    std::stringstream stream;
    while (true) {
        char buf[512];
        auto count = read(fd, buf, 512);
        if (count == -1) {
            if (errno == EAGAIN) {
                return stream.str();
            }
        } else if (count == 0) {
            close(fd);
            return stream.str();
        }

        stream << buf;
    }
}

void Server::work(std::string root) {
    while (true) {
        if (done) {
            break;
        }
        int fd;
        mu->lock();

        if (q.empty() || done) {
            mu->unlock();
            continue;
        }

        fd = q.front();
        q.pop();
        mu->unlock();

        auto data = read_data(fd);
        if (data == "") {
            auto response = new HttpResponse(STATUS_NOT_FOUND);
            std::string resStringData = response->toString();
            write_data(fd, resStringData);
            delete(response);
            close(fd);
            return;
        }
        auto req = new HttpRequest(data);
        HttpResponse *response;

        auto method = req->getMethod();
        if (method == "GET" || method == "HEAD") {
            int statusCode;
            auto fileManager = new FileManager(root + req->getPath());
            if (!fileManager->isExists()) {
                statusCode = STATUS_NOT_FOUND;
            } else {
                if (fileManager->isForbidden()) {
                    statusCode = STATUS_FORBIDDEN;
                } else {
                    statusCode = STATUS_OK;
                }
            }

            response = new HttpResponse(statusCode);
            auto filebuf = fileManager->getFileBuf();

            if (method == "GET") {
                response->setBody(filebuf);
            }

            response->setHeader("Content-Type: ", fileManager->getContentType());
            response->setHeader("Content-Length: ", std::to_string(filebuf.size()));

            delete(fileManager);
        } else {
            response = new HttpResponse(STATUS_NOT_ALLOWED);
        }

        std::string resStringData = response->toString();
        write_data(fd, resStringData);
        std::cout << req->getPath() << std::endl << response->getHeaders() << std::endl;

        delete(req);
        delete(response);
        close(fd);
    }
}

void Server::write_data(int fd, std::string buf) {
    write(fd, buf.c_str(), buf.size());
}

Server::~Server() {
    std::cout << "Distructor" << std::endl;
    for (int i = 0; i < maxThreads; i++) {
        workers[i].join();
    }

    close(socketFd);
}