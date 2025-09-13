add_executable(AmethystTest test.cpp)

target_link_libraries(AmethystTest PRIVATE Amethyst GTest::gtest GTest::gtest_main)

add_test(NAME AmethystTest COMMAND AmethystTest)