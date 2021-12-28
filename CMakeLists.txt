cmake_minimum_required(VERSION 3.10)

set(SRCS idlog.c)

add_library(idlog STATIC ${SRCS})
target_include_directories(idlog PUBLIC ./)
