#include <SFML/Window.hpp>
#include <thread>

#include "Bot.hpp"

int main() {
    Bot gege("bot.json");

    auto network = std::thread([&](){
        gege.run();
    });

    auto input = std::thread([&](){
        while (true) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                gege.stop();
                break;
            }
        }
    });

    network.join();
    input.join();

    return 0;
}