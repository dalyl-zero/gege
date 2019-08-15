cmake_minimum_required(VERSION 3.14)
project(gege)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS ON)

find_package(SFML REQUIRED COMPONENTS network)
find_package(Boost REQUIRED COMPONENTS system log)
find_package(Threads REQUIRED)
find_package(nlohmann_json)

add_executable(${PROJECT_NAME} src/main.cpp src/Client.cpp src/Client.hpp src/Bot.cpp src/Bot.hpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} sfml-network Threads::Threads nlohmann_json::nlohmann_json)