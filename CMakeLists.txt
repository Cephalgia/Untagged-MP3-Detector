cmake_minimum_required(VERSION 3.8)
project(UntaggedMP3Detector)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp MainWindow.h MainWindow.cpp tag.h tag.cpp)

set(BOOST_ROOT /home/cephalgia/Tools/boost_1_65_0)
set(CMAKE_AUTOMOC ON)

find_package(Boost 1.65.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(UntaggedMP3Detector ${SOURCE_FILES})

if(Boost_FOUND)

    target_link_libraries(UntaggedMP3Detector ${Boost_LIBRARIES})

endif()

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

qt5_use_modules( UntaggedMP3Detector Core Widgets Gui )
