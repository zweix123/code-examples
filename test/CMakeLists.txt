enable_testing()
include(GoogleTest)

add_executable(warmup warmup.cpp)
target_link_libraries(warmup GTest::gtest_main)

add_subdirectory(./HashTable)
add_subdirectory(./ObjPrint)
