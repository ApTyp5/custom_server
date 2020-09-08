//
// Created by pycnick on 06.09.2020.
//

#ifndef WEBSERVER_CONFIG_H
#define WEBSERVER_CONFIG_H

#include <tuple>


namespace config {

    std::tuple<std::string_view, std::string_view, uint16_t, size_t> parse_args(char* argv[]);
    std::tuple<size_t, size_t, std::string> parse_config(const std::string_view& config_path);

} // namespace config

#endif //WEBSERVER_CONFIG_H
