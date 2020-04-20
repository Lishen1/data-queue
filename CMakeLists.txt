cmake_minimum_required(VERSION 3.14)
set(target data-queue)
project( ${target})

set(root ${CMAKE_CURRENT_SOURCE_DIR})

set(sources

 )

set(includes
    ${root}/data-queue.h
    ${root}/types.h
 )


source_group(TREE ${root} FILES ${sources} ${includes})