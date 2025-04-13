add_library(ImageProcessing STATIC
    Capture.cpp
    Processor.cpp
)

target_include_directories(
    ImageProcessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/Core 
)

target_link_libraries(ImageProcessing PUBLIC
    ${OpenCV_LIBS}
    Qt6::Core
)
