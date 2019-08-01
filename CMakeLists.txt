cmake_minimum_required(VERSION 3.14)
project(gege)

set(CMAKE_CXX_STANDARD 17)
SET(Boost_USE_STATIC_LIBS ON)

find_package(SFML REQUIRED COMPONENTS network)
find_package(Boost REQUIRED COMPONENTS system log)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/Client.cpp src/Client.hpp src/Config.cpp src/Config.hpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} sfml-network Threads::Threads)