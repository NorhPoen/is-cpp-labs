cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(BUILD_SHARED_LIBS OFF)

project(labwork_10_ISKorobov)

add_executable(labwork_10_ISKorobov  main.cpp DataRequests.h DataRequests.cpp
        ConsoleTable/CppConsoleTable.hpp)

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG 6af826d0bdb55e4b69e3ad817576745335f243ca)
FetchContent_MakeAvailable(json)

target_link_libraries(labwork_10_ISKorobov PRIVATE cpr::cpr)
target_link_libraries(labwork_10_ISKorobov PRIVATE nlohmann_json::nlohmann_json)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Config.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})