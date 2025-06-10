include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

include(GoogleTest)
gtest_discover_tests(AppTests)


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
