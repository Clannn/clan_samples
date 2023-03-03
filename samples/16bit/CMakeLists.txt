get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR} PATH)
get_filename_component(CATEGORY_NAME ${PARENT_DIR} NAME)

add_sample_with_tags(
        ID ${FOLDER_NAME}
        CATEGORY ${CATEGORY_NAME}
        AUTHOR "Hans-Kristian Arntzen"
        NAME "16bit"
        DESCRIPTION "Using VK_KHR_shader_float16_int8 to improve arithmetic throughput."
        SHADER_FILES_GLSL
                "16bit_arithmetic/visualize.vert"
                "16bit_arithmetic/visualize.frag"
                "16bit_arithmetic/compute_buffer.comp")