cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
SET(EXTRA_COMPONENT_DIRS core application)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(CPP_ESP)