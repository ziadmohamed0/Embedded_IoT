cmake_minimum_required(VERSION 3.10)
project(TASK_2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SRC 
    main.cpp 
    Library/Books/books.cpp
    Library/Magazines/Magazine.cpp
)

include_directories(Library/Items Library/Books Library/Magazine)
add_executable(${PROJECT_NAME} ${SRC})
