#include <thread>
#include <mutex>

#include "Client.hpp"

int main() {
    try {
        // Example
        Client gege{{"irc.sfml-dev.org", 6667}};

        std::mutex mutex;
        std::thread listener{[&]() {
            while (gege.is_connected()) {
                mutex.lock();
                gege.listen();
                mutex.unlock();
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