//
// Created by dalyl on 7/29/19.
//

#ifndef GEGE_CLIENT_HPP
#define GEGE_CLIENT_HPP

#include <boost/algorithm/string/trim.hpp>
#include <SFML/Network.hpp>
#include <iostream>

class Client {
public:
    Client() = default;
    Client(std::string_view addr, std::uint16_t port);
    ~Client();
    void connect(std::string_view addr, std::uint16_t port);
    void registration();
    void nick(std::string_view name);
    void user(std::string_view username, std::string_view hostname, std::string_view realname, std::string_view servername);
    void join(std::string_view channel);
    void msg(std::string_view target, std::string_view content);
    void quit(std::string_view quit_msg = "");
    void pong(std::string_view code);
    std::string listen();
    [[nodiscard]] bool is_connected() const;
    [[nodiscard]] bool is_registered() const;

private:
    void disconnect();
    void log() const;
    void log(std::string_view error);

private:
    template <typename... Ts>
    bool send(Ts ...args) {
        auto msg = (std::string{args} + " " + ...);
        boost::trim(msg);
        msg += "\r\n";
        m_logstr += msg;
        std::cout << msg;
        return m_socket.send(msg.c_str(), msg.size()) == sf::Socket::Done;
    }

private:
    sf::TcpSocket m_socket;
    std::string m_logstr;
    bool m_registered{};
    bool m_connected{};
};


#endif //GEGE_CLIENT_HPP
