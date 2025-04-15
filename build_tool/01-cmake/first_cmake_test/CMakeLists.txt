# standared step.
cmake_minimum_required(VERSION 3.10)

# variables
set(SRC_FILES main.cpp src/calc.cpp)
set(EXE_FILE output_file)
# project name 
project(HelloCmake)

# soucrces files
add_executable(${EXE_FILE} ${SRC_FILES})

# if conditions
if(EXISTS ${CMAKE_SOURCE_DIR}/main.cpp)
    message(STATUS "Files Exists")
else()
    message(FATAL_ERROR "Error :(")
endif()

# include another cmake file
add_subdirectory(${CMAKE_SOURCE_DIR}/inc)

# header directories
target_include_directories(${EXE_FILE} PUBLIC inc/)

# linking library
# target_link_libraries(${EXE_FILE} PUBLIC wifi.lip)
