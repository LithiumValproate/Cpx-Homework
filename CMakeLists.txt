cmake_minimum_required(VERSION 3.10)
project(CpxHomework)

file(GLOB CH "*.cpp")
file(GLOB CPPS "Cpp/*.cpp")
file(GLOB CP "C/*.c")
file(GLOB LUOGU "Luogu/*.cpp")
file(GLOB LC "Leetcode/*.cpp")

foreach(file ${CPPS})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file}
            Cpp/DS_5.cpp)
endforeach()
