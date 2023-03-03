cmake_minimum_required(VERSION 3.16)

# scan_dirs(
#     DIR ${CMAKE_CURRENT_SOURCE_DIR}
#     LIST SUB_DIRS)

# List of all samples
set(TOTAL_SAMPLE_ID_LIST)

# For each directory, add all the samples that exist within
function(add_sub_dirs DIRECTORY)
    scan_dirs(
        DIR ${DIRECTORY}
        LIST DIR_LIST)

    foreach(CURR_DIR ${DIR_LIST})
        if(EXISTS "${DIRECTORY}/${CURR_DIR}/CMakeLists.txt")
            add_subdirectory(${DIRECTORY}/${CURR_DIR})
            list(APPEND TOTAL_SAMPLE_ID_LIST ${CURR_DIR})
        else()
            add_sub_dirs(${DIRECTORY}/${CURR_DIR})
        endif()
        set(TOTAL_SAMPLE_ID_LIST ${TOTAL_SAMPLE_ID_LIST} PARENT_SCOPE)
    endforeach()
endfunction(add_sub_dirs)

add_sub_dirs(${CMAKE_CURRENT_SOURCE_DIR})

# foreach(SUB_DIR ${SUB_DIRS})
#     add_sub_dirs(${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR})
# endforeach()

# Order of the sample ids
set(ORDER_LIST
    "hello_triangle"
    "16bit"
    "afbc"
)

# Orders the sample ids by the order list above
set(ORDERED_LIST)
list(REMOVE_DUPLICATES ORDER_LIST)

# Add samples to ordered list based on the a predefined order
foreach(SAMPLE_ID ${ORDER_LIST})
    list(FIND ORDERED_LIST ${SAMPLE_ID} FOUND_SAMPLE)
    if(NOT ${FOUND_SAMPLE} LESS 0)
        list(APPEND ORDERED_LIST ${SAMPLE_ID})
    endif()
endforeach()

# Add the remaining samples to the ordered list that are not in the predefined order
foreach(SAMPLE_ID ${TOTAL_SAMPLE_ID_LIST})
    list(FIND ORDERED_LIST ${SAMPLE_ID} FOUND_SAMPLE)
    if(${FOUND_SAMPLE} LESS 0)
        list(APPEND ORDERED_LIST ${SAMPLE_ID})
    endif()
endforeach()

# Make sample list visible parent scope (required by clan_samples project)
set(TOTAL_SAMPLE_ID_LIST ${ORDERED_LIST} PARENT_SCOPE)

message(STATUS "TOTAL_SAMPLE_ID_LIST:--------------------------${TOTAL_SAMPLE_ID_LIST}")