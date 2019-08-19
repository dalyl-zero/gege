#include <SFML/Window.hpp>

#include "Bot.hpp"

int main() {
    Bot gege("bot.json");

    auto network_listener = gege.run();

    auto input_listener = std::thread([&](){
        while (true) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                gege.stop();
                break;
            }
        }
    });

    network_listener.join();
    input_listener.join();

    return 0;
}