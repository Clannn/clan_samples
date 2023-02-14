cmake_minimum_required(VERSION 3.16)

project(clan_samples LANGUAGES C CXX)

add_subdirectory(plugins)
add_subdirectory(apps)

add_executable(${PROJECT_NAME} WIN32 main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../framework
)

target_link_libraries(${PROJECT_NAME} PRIVATE framework)

# message(STATUS "MSVC is true? ${MSVC}")