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
    ~Client();
    void nick(std::string_view name);
    void user(std::string_view username, std::string_view hostname, std::string_view servername, std::string_view realname);
    void join(std::string_view channel);
    void msg(std::string_view target, std::string_view content);
    void quit(std::optional<std::string_view> last_msg);
    void listen();
    [[nodiscard]] bool is_connected() const;

private:
    void log() const;

private:
    sf::TcpSocket m_socket;
    std::string m_logstr;
    bool m_registered;
    bool m_connected;
};


#endif //GEGE_CLIENT_HPP
