cmake_minimum_required(VERSION 3.9)

# Target: NEWorld
add_custom_target(NEWorldCopyAsset ALL CopyAssets
        ${NEWORLD_PRODUCT_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        infinideas.gui
        )
file(GLOB_RECURSE SRC_NEWorld Source/*.*)
add_executable(NEWorld ${SRC_NEWorld})
target_compile_definitions(NEWorld
        PRIVATE -DNWAPIEXPORTS
        PRIVATE -DNWCompartmentLoggerPrefix="NEWorld")

# Link libraries for NEWorld
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
find_package(SDL2-image REQUIRED)
target_link_libraries(NEWorld Core Threads::Threads SDL2::SDL2 SDL2::SDL2_image GLEW::GLEW)
add_definitions(-DSDL_MAIN_HANDLED)
add_dependencies(NEWorld NEWorldCopyAsset)


