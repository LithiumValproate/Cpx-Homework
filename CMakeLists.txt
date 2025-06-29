cmake_minimum_required(VERSION 3.10)
project(CpxHomework)

file(GLOB 3RD "3rd/*.cpp")
file(GLOB 2ND "2nd/*.cpp")
file(GLOB 1ST "1st/*.c")
file(GLOB LUOGU "Luogu/*.cpp")
file(GLOB LC "Leetcode/*.cpp")

foreach(file ${CPPS})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})

endforeach()
