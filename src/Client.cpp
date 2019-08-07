//
// Created by dalyl on 7/29/19.
//

#include <boost/algorithm/string/replace.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>

#include "Client.hpp"

static constexpr std::size_t BUFFER_SIZE = 1024;
static const std::string_view LOG_PATH = "./log/";

Client::Client(const Config& config) : m_connected{false}, m_registered{false} {
    sf::Socket::Status status = m_socket.connect(config.addr(), config.port());
    std::stringstream port;
    port << config.port();
    if (status != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to connect to " + config.addr() + " at port " + port.str());
    }
    m_connected = true;
    m_logstr += config.addr() + " " + port.str() + "\n\n";
}

Client::~Client() {
    log();
}

void Client::nick(std::string_view name) {
    std::string msg = "NICK " + std::string{name} + "\r\n";
    m_logstr += msg;
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to change nickname to " + std::string{name});
    }
}

void Client::user(std::string_view username, std::string_view hostname, std::string_view servername,
                  std::string_view realname) {
    std::string msg = "USER " + std::string{username} + " " + std::string{hostname} + " " + std::string{servername} + " :" + std::string{realname} + "\r\n";
    m_logstr += msg;
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to set USER");
    }
}

void Client::join(std::string_view channel) {
    if (!m_registered) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::string msg = "JOIN #" + std::string{channel} + "\r\n";
    m_logstr += msg;
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to join channel #" + std::string{channel});
    }
}

void Client::msg(std::string_view target, std::string_view content) {
    std::string msg = "PRIVMSG " + std::string{target} + " :" + std::string{content} + "\r\n";
    m_logstr += msg;
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to send message to " + std::string{target});
    }
}

void Client::quit(std::string_view quit_msg) {
    std::string msg = "QUIT " + std::string{quit_msg} + "\r\n";
    m_logstr += msg;
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to QUIT");
    }
    m_connected = false;
    m_registered = false;
}

void Client::quit() {
    quit("");
}

void Client::pong(std::string_view code) {
    std::string msg = "PONG " + std::string{code};
    m_logstr += msg;
    if (m_socket.send(msg.c_str(), msg.size()) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to send PONG");
    }
}

void Client::listen() {
    char buffer[BUFFER_SIZE];
    std::size_t data_size;
    if (m_socket.receive(buffer, BUFFER_SIZE, data_size) != sf::Socket::Done) {
        throw std::runtime_error("Error: Unable to receive data");
    }
    std::string received{buffer, data_size};

    m_logstr += received;
    std::cout << received;

    using tokenizer_t = boost::tokenizer<boost::char_separator<char>>;
    boost::char_separator<char> sep{"\r\n"};
    tokenizer_t lines{received, sep};
    for (auto& line : lines) {
        boost::tokenizer<> tokens{line};
        for (auto token_iterator = tokens.begin(); token_iterator != tokens.end(); ++token_iterator) {
            if (*token_iterator == "PING") {
                auto code = ":" + *token_iterator++;
                pong(code);
            }
            else if (*token_iterator == "MODE") {
                m_registered = true;
            }
        }
    }
}

void Client::log() const {
    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename = boost::replace_all_copy(std::string{ctime(&timenow)}, " ", "_");

    boost::log::add_file_log(std::string{LOG_PATH} + filename);
    boost::log::core::get()->set_filter( boost::log::trivial::severity >= boost::log::trivial::info );

    BOOST_LOG_TRIVIAL(info) << m_logstr;
}

bool Client::is_connected() const {
    return m_connected;
}