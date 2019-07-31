//
// Created by dalyl on 7/29/19.
//

#ifndef GEGE_CLIENT_HPP
#define GEGE_CLIENT_HPP

#include <SFML/Network.hpp>
#include <exception>
#include <iostream>
#include <optional>
#include <string>

#include "Config.hpp"

class Client {
public:
    explicit Client(const Config& config);
    void nick(const std::string& name);
    void user(const std::string& username, const std::string& hostname, const std::string& servername, const std::string& realname);
    void join(const std::string& channel);
    void msg(const std::string& target, const std::string& content);
    void quit(std::optional<std::string> message);
    void listen();
    [[nodiscard]] bool is_connected() const;

private:
    sf::TcpSocket m_socket;
    bool m_connected;
    bool m_registered;
};


#endif //GEGE_CLIENT_HPP
