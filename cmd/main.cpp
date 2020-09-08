//
// Created by pycnick on 03.09.2020.
//

#include "../internal/server/Server.h"
#include "../internal/tools/Config.h"
#include <csignal>

auto [maxCPU, maxThreads, root] = config::parse_config("/home/pycnick/dev/custom_server/httpd.conf");
Server *app = new Server("4242", maxThreads, root);

void signalHandler(int sigNum) {
    delete(app);
}
int main() {
    std::signal(SIGTERM, signalHandler);

    app->runServer();
}

