enable_testing()
include(GoogleTest)

# include
include_directories(${PROJECT_SOURCE_DIR}/skip_list)

file(GLOB srouce_filepaths CONFIGURE_DEPENDS *.cpp)

foreach(source_filepath ${srouce_filepaths})
  get_filename_component(target_name ${source_filepath} NAME_WE)
  add_executable(${target_name} ${source_filepath})
  target_link_libraries(${target_name} GTest::gtest_main)
  gtest_discover_tests(${target_name})
endforeach()
