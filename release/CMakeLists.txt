cmake_minimum_required(VERSION 3.1)

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_DIR ./../src)

if(EXTRA_INCDIR)
    include_directories(BEFORE ${EXTRA_INCDIR})
endif()

if(EXTRA_LIBDIR)
    link_directories(${EXTRA_LIBDIR})
endif()

if(UNIX)
    link_libraries(dl)
endif()

if(MS_VC)
    add_definitions(-DNOMINMAX)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

list(APPEND CMAKE_MODULE_PATH ../cmake)

project(NEWorld)
add_definitions()

# For the whole project
include_directories(${SOURCE_DIR})

# Target: nwcore
file(GLOB_RECURSE SRC_NWCORE ${SOURCE_DIR}/client/*.cpp
                             ${SOURCE_DIR}/engine/*.cpp
                             ${SOURCE_DIR}/game/*.cpp
                             ${SOURCE_DIR}/server/*.cpp
                             ${SOURCE_DIR}/sync_service/*.cpp)

add_library(nwcore SHARED ${SRC_NWCORE})

SET_TARGET_PROPERTIES(nwcore PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(nwcore PROPERTIES SUFFIX "")
target_compile_definitions(nwcore
    PRIVATE -DSDL_MAIN_HANDLED
    PRIVATE -DNWCOREEXPORTS
    PRIVATE -DNWCompartmentLoggerPrefix="nwcore")

# Link libraries for nwcore
find_package(rpclib REQUIRED)
target_include_directories(nwcore PUBLIC ${RPCLIB_INCLUDE_DIR})
target_link_libraries(nwcore pthread ${RPCLIB_LIBS})
target_compile_definitions(nwcore PUBLIC ${RPCLIB_COMPILE_DEFINITIONS})

# Target: GUI
file(GLOB_RECURSE SRC_GUI ${SOURCE_DIR}/gui/*.cpp)
add_library(GUI SHARED ${SRC_GUI})
target_compile_definitions(GUI PRIVATE -DNWCompartmentLoggerPrefix="GUI")

# Link libraries for GUI
target_link_libraries(GUI nwcore pthread freetype SDL2 SDL2_image GLEW)
if(CMAKE_SYSTEM_NAME MATCHES Darwin)
    target_link_libraries(GUI "-framework OpenGL")
endif()

# Target: CLI
file(GLOB_RECURSE SRC_CLI ${SOURCE_DIR}/cli/*.cpp)
add_library(CLI SHARED ${SRC_CLI})
target_link_libraries(CLI nwcore)
target_compile_definitions(CLI PRIVATE -DNWCompartmentLoggerPrefix="CLI")

# Target: NEWorld
project(NEWorld)
add_executable(NEWorld ${SOURCE_DIR}/launchers/client.cpp)
target_link_libraries(NEWorld GUI nwcore)
target_compile_definitions(NEWorld PRIVATE -DNWCompartmentLoggerPrefix="NEWorld")

# Target: NEWorld_Shell
project(NEWorld_Shell)
add_executable(NEWorld_Shell ${SOURCE_DIR}/launchers/shell.cpp)
target_link_libraries(NEWorld_Shell CLI nwcore)
target_compile_definitions(NEWorld_Shell PRIVATE -DNWCompartmentLoggerPrefix="NEWorld_Shell")

add_subdirectory(./plugins)
