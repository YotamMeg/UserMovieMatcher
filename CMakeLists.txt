cmake_minimum_required(VERSION 3.24)
project(ex5_yotam267)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_yotam267
        Movie.cpp
        Movie.h
        print_main.cpp
        RecommendationSystem.h
        RecommendationSystemLoader.cpp
        RecommendationSystemLoader.h
        User.cpp
        User.h
        UsersLoader.cpp
        UsersLoader.h RecommendationSystem.cpp)
add_executable(test
        Movie.cpp
        Movie.h
        presubmit.cpp
        RecommendationSystem.h
        RecommendationSystemLoader.cpp
        RecommendationSystemLoader.h
        User.cpp
        User.h
        UsersLoader.cpp
        UsersLoader.h RecommendationSystem.cpp)
