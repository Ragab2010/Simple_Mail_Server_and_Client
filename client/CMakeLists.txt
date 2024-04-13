cmake_minimum_required(VERSION 3.0)
project(SimpleMailClient)

set(CMAKE_CXX_STANDARD 17)

# Find JSONCPP package
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

# Include directories
include_directories(${JSONCPP_INCLUDE_DIRS})

# Client executable
add_executable(client
    client.cpp
    ClientSource.cpp
    JsonHandler.cpp
)

# Link libraries
target_link_libraries(client
    ${JSONCPP_LIBRARIES}
)
