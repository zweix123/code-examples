include_directories(${PROJECT_SOURCE_DIR}/ObjPrint)

file(GLOB srouce_filepaths CONFIGURE_DEPENDS *.cpp)

foreach(source_filepath ${srouce_filepaths})
    # message(${source_filepath})
    string(REGEX MATCH "ObjPrint/.*" target_name ${source_filepath})

    # message(${target_name})
    string(REGEX REPLACE "ObjPrint/" "" target_name ${target_name})

    # message(${target_name})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})

    # message(${target_name})
    add_executable(${target_name} ${source_filepath})
    target_link_libraries(${target_name} GTest::gtest_main)
    gtest_discover_tests(${target_name})
endforeach()
