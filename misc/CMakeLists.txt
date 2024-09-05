enable_testing()
include(GoogleTest)

# include
include_directories(${PROJECT_SOURCE_DIR}/obj_print)

file(GLOB srouce_filepaths CONFIGURE_DEPENDS *.cpp)

foreach(source_filepath ${srouce_filepaths})
  # message(${source_filepath})
  string(REGEX MATCH "misc/.*" target_name ${source_filepath})

  # message(${target_name})
  string(REGEX REPLACE "misc/" "" target_name ${target_name})

  # message(${target_name})
  string(REGEX REPLACE ".cpp" "" target_name ${target_name})

  # message(${target_name})
  add_executable(${target_name} ${source_filepath})
endforeach()
