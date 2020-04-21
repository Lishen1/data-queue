cmake_minimum_required(VERSION 3.14)
set(target data_queue)
project( ${target})

set(root ${CMAKE_CURRENT_SOURCE_DIR})

set(sources

 )

set(includes
    ${root}/data_queue.h
    ${root}/types.h
    ${root}/ring_buffer.h
 )

source_group(TREE ${root} FILES ${sources} ${includes})

add_library(${target} INTERFACE)

target_include_directories(${target} INTERFACE ${root})