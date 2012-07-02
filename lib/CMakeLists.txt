# add includes to this project
include_directories(${CMAKE_SOURCE_DIR}/lib/inc)

# add source code files to this project
file(GLOB ALL_SOURCES "src/*.cpp")
file(GLOB ALL_HEADERS "inc/*.h")

# tell CMake to create a static library
add_library(lib ${ALL_SOURCES} ${ALL_HEADERS})

