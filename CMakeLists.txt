cmake_minimum_required(VERSION 3.16)
project(FireFly)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt Meta-Object Compiler
set(CMAKE_AUTOMOC ON)

# Ensure CMake can find Qt installed via QTDIR
set(CMAKE_PREFIX_PATH "$ENV{QTDIR}/lib/cmake")

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

# Add subdirectories
add_subdirectory(src)

# Create main app executable
add_executable(FireFlyApp src/Core/main.cpp)

# Link everything together
target_link_libraries(FireFlyApp
    Qt6::Widgets
    ${OpenCV_LIBS}
    Core
    ImageProcessing
    GUI
)

# Output binary to bin/
set_target_properties(FireFlyApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
