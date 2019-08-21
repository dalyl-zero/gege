//
// Created by dalyl on 7/29/19.
//

#include <boost/algorithm/string/split.hpp>
#include <iostream>

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
}

void Bot::run() {
    m_client.connect(m_addr, m_port);
    m_client.nick(m_nick);
    m_client.user(m_nick, "0", "*", m_nick);
    bool joined{false};

    while (m_client.is_connected()) {
        parse(m_client.listen());

        if (m_client.is_registered() && !joined) {
            m_client.join(m_channel);
            joined = true;
        }
    }
}

void Bot::stop() {
    m_client.quit();
}

void Bot::parse(const std::string& received) {
    std::vector<std::string> tokens;
    boost::split(tokens, received, boost::is_space());

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "PING") {
            m_client.pong(tokens[i + 1]);
            ++i;
        }
        else if (tokens[i] == "MODE") {
            m_client.registration();
        }
    }
}