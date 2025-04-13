add_library(Core STATIC
    AppConfig.cpp
)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Core PUBLIC Qt6::Core)
