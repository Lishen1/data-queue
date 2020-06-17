project(examples)


add_executable(create_queue create_queue.cpp)
target_link_libraries(create_queue PRIVATE data_queue)
