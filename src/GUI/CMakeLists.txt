add_library(GUI STATIC
    MainWindow.cpp
)

target_include_directories(
    GUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/ImageProcessing
    ${CMAKE_SOURCE_DIR}/src/Core

)

target_link_libraries(GUI PUBLIC
    Qt6::Widgets
    ImageProcessing
    Core
)
