cmake_minimum_required(VERSION 3.28)
project(fiveStagePipeline)

set(CMAKE_CXX_STANDARD 20)

include_directories(fiveStagePipeline)

add_executable(code fiveStagePipeline/main.cpp
        fiveStagePipeline/src/Memory.cpp
        fiveStagePipeline/src/ProgramCounter.cpp
        fiveStagePipeline/src/ArithmeticLogicUnit.cpp
        fiveStagePipeline/src/LoadData.cpp
        fiveStagePipeline/src/RegisterFile.cpp
        fiveStagePipeline/src/CPU.cpp
        fiveStagePipeline/src/Decoder.cpp
        fiveStagePipeline/src/LoadStoreBuffer.cpp
        fiveStagePipeline/src/Predictor.cpp
        fiveStagePipeline/src/ReorderBuffer.cpp
        fiveStagePipeline/src/ReservationStation.cpp)
