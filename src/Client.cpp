//
// Created by dalyl on 7/29/19.
//

#include <boost/algorithm/string/replace.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

#include "Client.hpp"

static constexpr std::size_t BUFFER_SIZE = 1024;
static const std::string_view LOG_DIR = "./log/";

Client::Client(std::string_view addr, unsigned short port) : m_connected{false}, m_registered{false} {
    connect(addr, port);
}

Client::~Client() {
    log();
}

void Client::connect(std::string_view addr, unsigned short port) {
    if (!m_connected) {
        sf::Socket::Status status = m_socket.connect(std::string{addr}, port);
        std::stringstream ss;
        ss << port;
        if (status != sf::Socket::Done) {
            std::cerr << "Error: Unable to connect to " + std::string{addr} + " at port " + ss.str() << std::endl;
        }
        m_connected = true;
        m_logstr += std::string{addr} + " " + ss.str() + "\n\n";
    }
}

void Client::registration() {
    m_registered = true;
}

void Client::nick(std::string_view name) {
    if (!send("NICK", name)) {
        std::cerr << "Error: Unable to change nickname to " + std::string{name} << std::endl;
    }
}

void Client::user(std::string_view username, std::string_view hostname, std::string_view servername,
                  std::string_view realname) {
    if (!send("USER", username, hostname, servername, realname)) {
        std::cerr << "Error: Unable to set USER" << std::endl;
    }
}

void Client::join(std::string_view channel) {
    if (!send("JOIN", channel)) {
        std::cerr << "Error: Unable to join channel #" + std::string{channel} << std::endl;
    }
}

void Client::msg(std::string_view target, std::string_view content) {
    if (!send("PRIVMSG", target, content)) {
        std::cerr << "Error: Unable to send message to " + std::string{target} << std::endl;
    }
}

void Client::quit(std::string_view quit_msg) {
    if (!send("QUIT", quit_msg)) {
        std::cerr << "Error: Unable to QUIT" << std::endl;
    }
    m_connected = false;
    m_registered = false;
}

void Client::pong(std::string_view code) {
    if (!send("PONG", code)) {
        std::cerr << "Error: Unable to send PONG" << std::endl;
    }
}

std::string_view Client::listen() {
    char buffer[BUFFER_SIZE];
    std::size_t data_size;
    if (m_socket.receive(buffer, BUFFER_SIZE, data_size) != sf::Socket::Done) {
        std::cerr << "Error: Unable to receive data" << std::endl;
    }
    std::string received{buffer, data_size};

    m_logstr += received;
    std::cout << received;

    return received;
}

void Client::log() const {
    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename = boost::replace_all_copy(std::string{ctime(&timenow)}, " ", "_");

    boost::log::add_file_log(std::string{LOG_DIR} + filename);
    boost::log::core::get()->set_filter( boost::log::trivial::severity >= boost::log::trivial::info );

    BOOST_LOG_TRIVIAL(info) << m_logstr;
}

bool Client::is_connected() const {
    return m_connected;
}

bool Client::is_registered() const {
    return m_registered;
}