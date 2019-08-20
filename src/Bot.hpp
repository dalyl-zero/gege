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
    void parse(std::string_view received);

private:
    std::string m_addr;
    unsigned short m_port;
    std::string m_nick;
    std::string m_channel;
    nlohmann::json m_config;
    Client m_client;
};


#endif //GEGE_BOT_HPP
