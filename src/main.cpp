#include "Bot.hpp"

int main() {
    Bot gege("bot.json");

    auto listener = gege.run();

    listener.join();

    return 0;
}