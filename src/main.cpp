#include <thread>
#include <mutex>

#include "Client.hpp"

int main() {
    try {
        // Example
        Client gege{{"irc.freenode.net", 6667}};

        std::mutex mutex;
        std::thread listener{[&]() {
            while (gege.is_connected()) {
                std::lock_guard<std::mutex> lock{mutex};
                gege.listen();
            }
        }};

        gege.nick("dalyl");
        gege.user("dalyl", "0", "*", "dalyl");
        gege.join("blender");
        gege.msg("#blender", "Hello, world!");
        gege.quit();

        listener.join();
    }
    catch (const std::exception& e){
        e.what();
    }
    return 0;
}