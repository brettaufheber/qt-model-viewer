cmake_minimum_required(VERSION 3.0)

# set project target
project(QtModelViewer VERSION 1.1.1)

add_definitions(-DPROJECT="${PROJECT_NAME}")
add_definitions(-DVERSION_MAJOR="${PROJECT_VERSION_MAJOR}")
add_definitions(-DVERSION_MINOR="${PROJECT_VERSION_MINOR}")
add_definitions(-DVERSION_PATCH="${PROJECT_VERSION_PATCH}")
add_definitions(-DAUTHOR="Eric Löffler")
add_definitions(-DWEB="https://github.com/brettaufheber/qt_model_viewer")
add_definitions(-DEMAIL="eric.loeffler@opalia.systems")
add_definitions(-DCOPYRIGHT="Copyright © 2016")

# find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# build with position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# find Qt5 components
find_package(Qt5 COMPONENTS Widgets OpenGL UiTools)

# add includes for Qt5 components
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5UiTools_INCLUDE_DIRS})
include_directories(${Qt5OpenGL_INCLUDE_DIRS})

# set compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# go to sub dirsectories
subdirs(main lib)

