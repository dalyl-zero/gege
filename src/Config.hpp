//
// Created by dalyl on 7/29/19.
//

#ifndef GEGE_CONFIG_HPP
#define GEGE_CONFIG_HPP

#include <string>

class Config {
public:
    Config(const std::string_view& addr, unsigned short port);
    [[nodiscard]] const std::string& addr() const;
    [[nodiscard]] unsigned short port() const;

private:
    std::string m_addr;
    unsigned short m_port;
};


#endif //GEGE_CONFIG_HPP
