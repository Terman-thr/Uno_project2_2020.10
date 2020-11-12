cmake_minimum_required(VERSION 3.10)
project(p2 C)

set(CMAKE_C_STANDARD 11)

add_executable(p2 main.c UI.c UI.h OperateCard.c OperateCard.h Const.h test.c test.h Const.c PlayPart.c PlayPart.h)