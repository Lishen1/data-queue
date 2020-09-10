cmake_minimum_required(VERSION 3.14)
project( data_queue_tests)

enable_testing()

add_executable ( data_queue_tests unit_test.cpp )
target_link_libraries ( data_queue_tests PRIVATE data_queue_features_util )
add_data_queue_tests_dependency (data_queue_tests)
add_test( data_queue_tests data_queue_tests )

