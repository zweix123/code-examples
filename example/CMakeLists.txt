include_directories(${PROJECT_SOURCE_DIR}/ObjPrint)

file(GLOB srouce_filepaths CONFIGURE_DEPENDS *.cpp)

foreach(source_filepath ${srouce_filepaths})
    # message(${source_filepath})
    string(REGEX MATCH "example/.*" target_name ${source_filepath})

    # message(${target_name})
    string(REGEX REPLACE "example/" "" target_name ${target_name})

    # message(${target_name})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})

    # message(${target_name})
    add_executable(${target_name} ${source_filepath})
endforeach()
