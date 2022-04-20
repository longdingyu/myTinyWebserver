cmake_minimum_required(VERSION 3.20)
project(TinyWebServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(TinyWebServer main.cpp config.cpp http/http_conn.h server.cpp timer/lst_timer.cpp timer/lst_timer.h log/log.cpp log/log.h)
