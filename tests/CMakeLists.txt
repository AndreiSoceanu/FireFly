add_executable(AppTests
    test_AppConfig.cpp
)

target_link_libraries(AppTests
    gtest
    Core
    ImageProcessing
    Qt6::Core
)

add_test(NAME AppTests COMMAND AppTests)
