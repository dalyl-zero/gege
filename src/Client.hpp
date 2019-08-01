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
    void nick(const std::string& name);
    void user(const std::string& username, const std::string& hostname, const std::string& servername, const std::string& realname);
    void join(const std::string& channel);
    void msg(const std::string& target, const std::string& content);
    void quit(std::optional<std::string> last_msg);
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
