cmake_minimum_required(VERSION 3.14)
project(foolisRcon)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json REQUIRED)

add_executable(foolisRcon
    main.cpp
    RconClient.cpp
    utils/Network.cpp
    utils/ArgParser.cpp
    utils/ConfigManager.cpp
)

target_include_directories(foolisRcon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/utils)
target_link_libraries(foolisRcon PRIVATE nlohmann_json::nlohmann_json)

