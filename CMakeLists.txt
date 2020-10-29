cmake_minimum_required(VERSION 3.17)
project(p2 C)

set(CMAKE_C_STANDARD 11)

add_executable(p2 main.c poker_total_card.h UI.c UI.h card_operator.c card_operator.h)