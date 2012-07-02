# add includes to this project
include_directories(${CMAKE_SOURCE_DIR}/lib/inc)

# add source code files to this project
file(GLOB ALL_SOURCES "src/*.cpp")
file(GLOB ALL_HEADERS "inc/*.h")

# add resource files to this project
set(RESOURCE ${CMAKE_SOURCE_DIR}/resource.qrc)
qt5_add_resources(RESOURCE_ADDED ${RESOURCE})

# tell CMake to create an executable
add_executable(main ${ALL_SOURCES} ${ALL_HEADERS} ${RESOURCE_ADDED})

# link libraries and other project dependencies
target_link_libraries(main lib GL GLU dl)

# bind Qt5 modules
qt5_use_modules(main Widgets OpenGL UiTools)

