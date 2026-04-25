cmake_minimum_required(VERSION 3.10)
project(BlackCloverGame VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML 3 (install via: brew install sfml  OR  sudo apt install libsfml-dev)
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)
# Collect all .cpp source files automatically
file(GLOB SOURCES "*.cpp")

# Define executable
add_executable(BlackCloverGame ${SOURCES})

# Explicitly add Homebrew include path for IDEs
target_include_directories(BlackCloverGame PRIVATE /opt/homebrew/include)

# Link SFML libraries
target_link_libraries(BlackCloverGame PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# Ensure includes are correctly propagated to IDEs
target_include_directories(BlackCloverGame PUBLIC 
    ${SFML_INCLUDE_DIR}
    /opt/homebrew/include
    /usr/local/include
)

# FIX: Copy the assets folder to the build output directory automatically.
# Without this, texture/font load calls with relative paths like "assets/player.png"
# fail at runtime because the working directory is the build folder, not the source folder.
add_custom_command(TARGET BlackCloverGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:BlackCloverGame>/assets"
    COMMENT "Copying assets to build directory"
    VERBATIM
)
