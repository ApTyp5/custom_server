project(webserver)

cmake_minimum_required(VERSION 3.2)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(webserver ./cmd/main.cpp
        internal/server/Server.cpp
        internal/http/HttpRequest.cpp
        internal/http/HttpResponse.cpp
        internal/tools/FileManager.cpp
        internal/tools/Config.cpp)