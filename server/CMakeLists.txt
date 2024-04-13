cmake_minimum_required(VERSION 3.0)
project(SimpleMailServer)

set(CMAKE_CXX_STANDARD 17)

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Find JSONCPP package
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

# Include directories
include_directories(${SQLite3_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIRS})

# Add executable for server
add_executable(server
    Server.cpp
    ServerSocket.cpp
    SQLiteDatabase.cpp
    TcpClientRequestHandler.cpp
    HttpClientRequestHandler.cpp
    JsonHandler.cpp
)

# # Set the output directory to the server directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Link libraries
target_link_libraries(server
    pthread
    ${SQLite3_LIBRARIES}
    ${JSONCPP_LIBRARIES}
)
