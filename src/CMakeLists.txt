cmake_minimum_required(VERSION 3.16)
project(breakout2d)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

message(STATUS "MY_VAR is: ${CMAKE_SOURCE_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/../include/)

# add library directories
link_directories(${CMAKE_SOURCE_DIR}/../libs/)

# the .c extension is just for glad.c file 
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.c")

add_executable(main MACOSX_BUNDLE ${SOURCES})

# glm is header only lib so we only add glfw3
target_link_libraries(main
    glfw3    # Use 'glfw' or 'glfw3' based on your built lib name
    "-framework OpenGL" # following lines are apple's platform dependent stuff, dont worry about it kitten :)
    "-framework Cocoa"
    "-framework CoreFoundation"
    "-framework IOKit"
    "-framework CoreVideo"
)