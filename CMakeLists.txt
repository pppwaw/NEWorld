cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(SRCDIR ./src)

option(LINK_BOOST "Enable link compiler switch of Boost library instead of auto-link" OFF)
option(EXECUTABLE "Link the executable for client and server" OFF)

if(LINK_BOOST STREQUAL ON)
    link_libraries(boost_system boost_filesystem boost_date_time)
endif()

if(UNIX)
    link_libraries(dl)
endif(UNIX)

if(MS_VC)
    add_definitions(-DNOMINMAX)
endif(MS_VC)

add_definitions(-DSDL_MAIN_HANDLED)

# ---
project(uilib)
aux_source_directory(${SRCDIR}/uilib/src SRC_UILIB)
add_library(uilib STATIC ${SRC_UILIB})
target_link_libraries(uilib GL GLEW SDL2 SDL2main SDL2_image)
target_include_directories(uilib PUBLIC ${SRCDIR}/uilib/include)
# ---
project(nwshared)
aux_source_directory(${SRCDIR}/shared SRC_SHARED)
add_library(nwshared STATIC ${SRC_SHARED})
# ---
project(nwclient)
aux_source_directory(${SRCDIR}/client SRC_CLIENT)
if(EXECUTABLE STREQUAL ON)
    add_executable(NEWorld ${SRC_CLIENT})
else()
    add_library(NEWorld SHARED ${SRC_CLIENT})
endif()
target_link_libraries(NEWorld pthread nwshared uilib freetype)
target_include_directories(NEWorld PUBLIC ${SRCDIR}/shared PUBLIC)
# ---
project(nwserver)
aux_source_directory(${SRCDIR}/server SRC_SERVER)
if(EXECUTABLE STREQUAL ON)
    add_executable(NEWorldServer ${SRC_SERVER})
else()
    add_library(NEWorldServer SHARED ${SRC_SERVER})
endif()
target_link_libraries(NEWorldServer nwshared pthread)
target_include_directories(NEWorldServer PUBLIC ${SRCDIR}/shared)
# ---
project(launcher)
aux_source_directory(${SRCDIR}/launcher SRC_LAUNCHER)
add_executable(launcher ${SRC_LAUNCHER})
target_include_directories(launcher PUBLIC ${SRCDIR}/shared)

add_dependencies(NEWorld nwshared)
add_dependencies(NEWorldServer nwshared)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-attributes")
endif()
