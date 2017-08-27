cmake_minimum_required(VERSION 3.4.1)

include_directories(SYSTEM src/main/cpp/)

file(GLOB_RECURSE SRC_LIST src/main/cpp/*.cpp)
file(GLOB_RECURSE HEADERS_LIST src/main/cpp/*.h)

add_library(game SHARED ${SRC_LIST} ${HEADERS_LIST})

find_library(log-lib log)

target_link_libraries(game ${log-lib} GLESv2)