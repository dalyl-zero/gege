//
// Created by dalyl on 7/29/19.
//

#include "Config.hpp"

Config::Config(const std::string_view& addr, unsigned short port) : m_addr{addr}, m_port{port} {
}

const std::string& Config::addr() const {
    return m_addr;
}

unsigned short Config::port() const {
    return m_port;
}