cmake_minimum_required(VERSION 3.26)

project(Abelian-sandpile-model)

set(CMAKE_CXX_STANDARD 17)


add_executable(Abelian-sandpile-model main.cpp
        Image.h
        Image.cpp
        Parser.h
        Parser.cpp
        Sandpile.h
        Sandpile.cpp
        Timer.h)