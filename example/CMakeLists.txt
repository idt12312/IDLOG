cmake_minimum_required(VERSION 3.1.0)

project(idlog_example)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(../)
file(GLOB SRCS *.c ../*.c)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
