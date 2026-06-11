cmake_minimum_required(VERSION 3.25)
project(solution_submission)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(solution_submission
        Guest.cpp
        SegmentationResort26b1.cpp
        main26b1.cpp)
