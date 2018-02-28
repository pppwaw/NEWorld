cmake_minimum_required(VERSION 3.9)

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_DIR ./../src)

if(UNIX)
    link_libraries(dl)
endif()

list(APPEND CMAKE_MODULE_PATH ../cmake)

project(NEWorld)

# Set Build Output Path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SDK/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Product)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Product)

# For the whole project
include_directories(${SOURCE_DIR})
find_package(Threads REQUIRED)

# Target: Core
file(GLOB_RECURSE SRC_CORE ${SOURCE_DIR}/client/*.cpp
                             ${SOURCE_DIR}/Common/*.cpp
                             ${SOURCE_DIR}/game/*.cpp
                             ${SOURCE_DIR}/server/*.cpp
                             ${SOURCE_DIR}/sync_service/*.cpp)

add_library(Core SHARED ${SRC_CORE})
target_compile_definitions(Core
    PRIVATE -DSDL_MAIN_HANDLED
    PRIVATE -DNWCOREEXPORTS
    PRIVATE -DNWCompartmentLoggerPrefix="Core")

# Link libraries for Core
find_package(rpclib REQUIRED)
target_include_directories(Core PUBLIC ${RPCLIB_INCLUDE_DIR})
target_link_libraries(Core Threads::Threads ${RPCLIB_LIBS})
target_compile_definitions(Core PUBLIC ${RPCLIB_COMPILE_DEFINITIONS})

# Target: GUI
file(GLOB_RECURSE SRC_GUI ${SOURCE_DIR}/gui/*.cpp)
add_library(GUI SHARED ${SRC_GUI})
target_compile_definitions(GUI PRIVATE -DNWCompartmentLoggerPrefix="GUI")

# Link libraries for GUI
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_include_directories(GUI ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(GUI Core Threads::Threads
    ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})

# Target: CLI
file(GLOB_RECURSE SRC_CLI ${SOURCE_DIR}/cli/*.cpp)
add_library(CLI SHARED ${SRC_CLI})
target_link_libraries(CLI Core)
target_compile_definitions(CLI PRIVATE -DNWCompartmentLoggerPrefix="CLI")

# Target: NEWorld
project(NEWorld)
add_executable(NEWorld ${SOURCE_DIR}/launchers/client.cpp)
target_link_libraries(NEWorld GUI Core)
target_compile_definitions(NEWorld PRIVATE -DNWCompartmentLoggerPrefix="NEWorld")

# Target: NEWorldShell
project(NEWorldShell)
add_executable(NEWorldShell ${SOURCE_DIR}/launchers/shell.cpp)
target_link_libraries(NEWorldShell CLI Core)
target_compile_definitions(NEWorldShell PRIVATE -DNWCompartmentLoggerPrefix="NEWorldShell")

add_subdirectory(./plugins)
