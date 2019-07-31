#include <thread>

#include "Client.hpp"

int main() {
    try {
        // Example
        Client gege{{"irc.sfml-dev.org", 6667}};

        std::thread listener{[&]() {
            while (gege.is_connected()) {
                gege.listen();
            }
        }};

        gege.nick("gege");
        gege.user("gege", "0", "*", "gege");
        gege.join("general");
        gege.msg("#general", "Hello, world!");
        gege.quit(std::nullopt);

        listener.join();
    }
    catch (const std::exception& e){
        e.what();
    }

    return 0;
}