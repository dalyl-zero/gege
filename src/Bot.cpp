//
// Created by dalyl on 7/29/19.
//

#include <boost/tokenizer.hpp>
#include <iostream>
#include <mutex>

#include "Bot.hpp"

static const std::string_view CONFIG_DIR = "./config/";

Bot::Bot(std::string_view filename) {
    std::string config_path = std::string{CONFIG_DIR} + std::string{filename};
    std::ifstream json_data{config_path};
    if (json_data.is_open()) {
        json_data >> m_config;
    }
    else {
        std::cerr << "Error: " + config_path + " does not exist" << std::endl;
    }

    m_addr = m_config["addr"];
    m_port = m_config["port"];
    m_nick = m_config["nick"];
    m_channel = m_config["channel"];

    m_client.connect(m_addr, m_port);
    m_client.nick(m_nick);
    m_client.user(m_nick, "0", "*", m_nick);
    m_client.join(m_channel);
}

std::thread Bot::run() {
    return std::thread{&Bot::exec, this};
}

void Bot::stop() {
    m_client.quit();
}

void Bot::exec() {
    std::mutex mutex;
    std::lock_guard<std::mutex> lock{mutex};
    while (m_client.is_connected()) {
        std::string_view m_received = m_client.listen();
        parse(m_received);
    }
}

void Bot::parse(std::string_view received) {
    using tokenizer_t = boost::tokenizer<boost::char_separator<char>>;

    boost::char_separator<char> sep{"\r\n"};
    tokenizer_t lines{received, sep};
    for (auto& line : lines) {
        boost::tokenizer<> tokens{line};
        for (auto token = tokens.begin(); token != tokens.end(); ++token) {
            if (*token == "PING") {
                auto code = ":" + *token++;
                m_client.pong(code);
            }
            else if (*token == "MODE") {
                m_client.registration();
            }
        }
    }
}