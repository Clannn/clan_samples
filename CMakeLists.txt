cmake_minimum_required(VERSION 3.16)

project(clan_samples)

# Add path for local cmake scripts
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bldsys/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/bldsys/cmake/module)

include(utils)
include(global_options)
include(sample_helper)

# Add third party libraries
add_subdirectory(third_party)

# Add framework
add_subdirectory(framework)

# Add samples
add_subdirectory(samples)

# Add app
add_subdirectory(app)