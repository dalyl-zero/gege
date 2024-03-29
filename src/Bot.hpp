//
// Created by dalyl on 7/29/19.
//

#ifndef GEGE_BOT_HPP
#define GEGE_BOT_HPP

#include <nlohmann/json.hpp>
#include <fstream>
#include <thread>

#include "Client.hpp"

class Bot {
public:
    explicit Bot(std::string_view filename);
    void run();
    void stop();

private:
    void parse(const std::string& received);

private:
    std::string m_addr;
    std::uint16_t m_port;
    std::string m_nick;
    std::string m_channel;
    nlohmann::json m_config;
    Client m_client;
};


#endif //GEGE_BOT_HPP
