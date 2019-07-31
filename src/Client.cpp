//
// Created by dalyl on 7/29/19.
//

#include <boost/algorithm/string.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "Client.hpp"

static constexpr std::size_t BUFFER_SIZE = 1024;

Client::Client(const Config& config) : m_connected{false}, m_registered{false} {
    sf::Socket::Status status = m_socket.connect(config.addr(), config.port());
    if (status != sf::Socket::Done) {
        std::stringstream port;
        port << config.port();
        throw std::runtime_error("Error: Unable to connect to " + config.addr() + " at port " + port.str());
    }
    m_connected = true;
}

void Client::nick(const std::string &name) {
    std::string msg = "NICK " + name + "\r\n";
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to change nickname to " + name);
    }
}

void Client::user(const std::string& username, const std::string& hostname, const std::string& servername,
                  const std::string& realname) {
    std::string msg = "USER " + username + " " + hostname + " " + servername + " :" + realname + "\r\n";
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to set USER");
    }
}

void Client::join(const std::string& channel) {
    if (!m_registered) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::string msg = "JOIN #" + channel + "\r\n";
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to join channel #" + channel);
    }
}

void Client::msg(const std::string& target, const std::string& content) {
    std::string msg = "PRIVMSG " + target + " " + content + "\r\n";
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to send message to " + target);
    }
}

void Client::quit(std::optional<std::string> log) {
    std::string msg = "QUIT ";
    if (log.has_value()) {
        msg += log.value();
    }
    msg += "\r\n";
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to QUIT");
    }
    m_connected = false;
    m_registered = false;
}

void Client::listen() {
    char buffer[BUFFER_SIZE];
    std::size_t data_size;
    if (m_socket.receive(buffer, BUFFER_SIZE, data_size) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to receive data");
    }
    std::string received{buffer, data_size};

    std::cout << received;

    std::vector<std::string> tokens;
    boost::split(tokens, received, boost::is_any_of(" ,\r\n"));
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "PING") {
            std::string msg = "PONG " + tokens[i + 1] + "\r\n";
            if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
                throw std::runtime_error("Error: Unable to send PONG");
            }
        }
        if (tokens[i] == "MODE") {
            m_registered = true;
        }
    }
}

bool Client::is_connected() const {
    return m_connected;
}