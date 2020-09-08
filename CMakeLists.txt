cmake_minimum_required(VERSION 3.14)

project( data_queue)

option(DATA_QUEUE_EXAMPLES "Should the examples to be." NO)
option(DATA_QUEUE_TESTS "Should the examples to be." YES)

include(cmake/warnings.cmake)
include(cmake/Dependency.cmake)


add_library( data_queue_features_util INTERFACE)
target_compile_features(data_queue_features_util INTERFACE cxx_std_17)
set_project_warinigs(data_queue_features_util)
target_include_directories(data_queue_features_util INTERFACE include)


add_library( data_queue INTERFACE )
target_link_libraries(data_queue INTERFACE data_queue_features_util)

message(STATUS "Examples enabled: ${DATA_QUEUE_EXAMPLES}")
message(STATUS "Tests enabled: ${DATA_QUEUE_TESTS}")

if ( DATA_QUEUE_EXAMPLES )
    add_subdirectory(examples)
endif()

if ( DATA_QUEUE_TESTS )
    add_subdirectory(tests)
endif()