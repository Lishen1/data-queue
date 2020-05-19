cmake_minimum_required(VERSION 3.14)

project( data_queue)

option(EXAMPLES_ENABLED "Should the examples to be." YES)

add_library( cppfeatures INTERFACE)
target_compile_features(cppfeatures INTERFACE cxx_std_17)

include(cmake/warnings.cmake)
add_library( cppwarnings INTERFACE)
set_project_warinigs(cppwarnings)

target_include_directories(cppfeatures INTERFACE include)

add_library( data_queue INTERFACE )
target_link_libraries(data_queue INTERFACE cppfeatures cppwarnings)

message(STATUS "Examples enabled: ${EXAMPLES_ENABLED}")

if ( EXAMPLES_ENABLED )
    add_subdirectory(examples)
endif()
