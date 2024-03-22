enable_testing()
include(GoogleTest)

include_directories(${PROJECT_SOURCE_DIR}/common)

add_executable(warmup warmup.cpp)
target_link_libraries(warmup GTest::gtest_main)
add_executable(common common.cpp)
target_link_libraries(common GTest::gtest_main)

add_subdirectory(./HashTable)
add_subdirectory(./ObjPrint)
add_subdirectory(./CacheImpl)
