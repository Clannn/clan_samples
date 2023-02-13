
cmake_minimum_required(VERSION 3.12)

set(SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(ROOT_DIR ${SCRIPT_DIR}/../..)

set(CMAKE_FILE ${SCRIPT_DIR}/template/sample/CMakeLists.txt.in)
set(SAMPLE_SOURCE_FILE ${SCRIPT_DIR}/template/sample/sample.cpp.in)
set(SAMPLE_HEADER_FILE ${SCRIPT_DIR}/template/sample/sample.h.in)

set(SAMPLE_NAME "" CACHE STRING "")
set(OUTPUT_DIR "${ROOT_DIR}/samples" CACHE PATH "")

# Only create a new sample if a name is given
if(NOT SAMPLE_NAME)
    message(FATAL_ERROR "Sample name cannot be empty.")
endif()

# Convert filename to accepted format
# insert an underscore before any upper case letter
string(REGEX REPLACE "(.)([A-Z][a-z]+)" "\\1_\\2" result ${SAMPLE_NAME})
# insert an underscore before any upper case letter
string(REGEX REPLACE "([a-z0-9])([A-Z])" "\\1_\\2" result ${result})   
# transform characters to lower case
string(TOLOWER ${result} SAMPLE_NAME_FILE)

configure_file(${CMAKE_FILE} ${OUTPUT_DIR}/${SAMPLE_NAME_FILE}/CMakeLists.txt @ONLY)
configure_file(${SAMPLE_SOURCE_FILE} ${OUTPUT_DIR}/${SAMPLE_NAME_FILE}/${SAMPLE_NAME_FILE}.cpp)
configure_file(${SAMPLE_HEADER_FILE} ${OUTPUT_DIR}/${SAMPLE_NAME_FILE}/${SAMPLE_NAME_FILE}.h)
