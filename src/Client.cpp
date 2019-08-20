//
// Created by dalyl on 7/29/19.
//

#include <boost/algorithm/string/replace.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <chrono>
#include <ctime>

#include "Client.hpp"

static constexpr std::size_t BUFFER_SIZE = 1024;
static const std::string_view LOG_DIR = "./log/";

Client::Client(std::string_view addr, std::uint16_t port) : m_connected{false}, m_registered{false} {
    connect(addr, port);
}

Client::~Client() {
    log();
}

void Client::connect(std::string_view addr, std::uint16_t port) {
    if (!m_connected) {
        sf::Socket::Status status = m_socket.connect(std::string{addr}, port);
        std::stringstream ss;
        ss << port;
        if (status != sf::Socket::Done) {
            log("connect to " + std::string{addr} + " at port " + ss.str());
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
        log("change nickname to " + std::string{name});
    }
}

void Client::user(std::string_view username, std::string_view hostname, std::string_view servername,
                  std::string_view realname) {
    if (!send("USER", username, hostname, servername, realname)) {
        log("set USER");
    }
}

void Client::join(std::string_view channel) {
    if (!send("JOIN", channel)) {
        log("JOIN " + std::string{channel});
    }
}

void Client::msg(std::string_view target, std::string_view content) {
    if (!send("PRIVMSG", target, content)) {
        log("send message to " + std::string{target});
    }
}

void Client::quit(std::string_view quit_msg) {
    if (!send("QUIT", quit_msg)) {
        log("QUIT");
    }
    else {
        disconnect();
    }
}

void Client::pong(std::string_view code) {
    if (!send("PONG", code)) {
        log("send PONG");
    }
}

std::string_view Client::listen() {
    char buffer[BUFFER_SIZE];
    std::size_t data_size;
    if (m_socket.receive(buffer, BUFFER_SIZE, data_size) != sf::Socket::Done) {
        log("receive data");
    }
    std::string received{buffer, data_size};

    m_logstr += received;
    std::cout << received;

    return received;
}

void Client::disconnect() {
    m_registered = false;
    m_connected = false;
}

void Client::log() const {
    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename = boost::replace_all_copy(std::string{ctime(&timenow)}, " ", "_");

    boost::log::add_file_log(std::string{LOG_DIR} + filename);
    boost::log::core::get()->set_filter( boost::log::trivial::severity >= boost::log::trivial::info );

    BOOST_LOG_TRIVIAL(info) << m_logstr;
}

void Client::log(std::string_view error) {
    std::string msg = "[ABORTED] Unexpected error when trying to " + std::string{error} + "\r\n";
    m_logstr += msg;
    disconnect();
}

bool Client::is_connected() const {
    return m_connected;
}

bool Client::is_registered() const {
    return m_registered;
}