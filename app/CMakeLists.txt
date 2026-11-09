cmake_minimum_required(VERSION 3.20.0)

list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../drivers/our_driver
                 ${CMAKE_CURRENT_BINARY_DIR}/our_driver)

target_sources(app PRIVATE src/main.cpp)
