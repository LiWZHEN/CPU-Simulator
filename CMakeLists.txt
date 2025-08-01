cmake_minimum_required(VERSION 3.28)
project(Tomasulo)

set(CMAKE_CXX_STANDARD 20)

include_directories(Tomasulo)

add_executable(code Tomasulo/main.cpp
        Tomasulo/src/Memory.cpp
        Tomasulo/src/ProgramCounter.cpp
        Tomasulo/src/ArithmeticLogicUnit.cpp
        Tomasulo/src/LoadData.cpp
        Tomasulo/src/RegisterFile.cpp
        Tomasulo/src/CPU.cpp
        Tomasulo/src/Decoder.cpp
        Tomasulo/src/LoadStoreBuffer.cpp
        Tomasulo/src/Predictor.cpp
        Tomasulo/src/ReorderBuffer.cpp
        Tomasulo/src/ReservationStation.cpp
        Tomasulo/src/Class.cpp)

# include_directories(singleStagePipeline)

# add_executable(singleStagePipeline singleStagePipeline/main.cpp
#         singleStagePipeline/src/Memory.cpp
#         singleStagePipeline/src/ProgramCounter.cpp
#         singleStagePipeline/src/InstructionDecoder.cpp
#         singleStagePipeline/src/InstructionDecoder.cpp
#         singleStagePipeline/src/ArithmeticLogicUnit.cpp
#         singleStagePipeline/src/BranchComparation.cpp
#         singleStagePipeline/src/LoadData.cpp
#         singleStagePipeline/src/RegFile.cpp
#         singleStagePipeline/src/Run.cpp)