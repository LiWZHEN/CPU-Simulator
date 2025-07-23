cmake_minimum_required(VERSION 3.28)
project(singleStagePipeline)

set(CMAKE_CXX_STANDARD 20)

include_directories(singleStagePipeline)

add_executable(singleStagePipeline singleStagePipeline/main.cpp
        singleStagePipeline/src/Memory.cpp
        singleStagePipeline/src/ProgramCounter.cpp
        singleStagePipeline/src/InstructionDecoder.cpp)
