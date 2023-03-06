get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR} PATH)
get_filename_component(CATEGORY_NAME ${PARENT_DIR} NAME)

add_sample_with_tags(
    ID ${FOLDER_NAME}
    CATEGORY ${CATEGORY_NAME}
    AUTHOR "clan"
    NAME "lighting"
    DESCRIPTION "Using multiple queues to async compute lighting."
    SHADER_FILES_GLSL
        "lighting/forward.vert"
        "lighting/forward.frag"
        "lighting/shadow.vert"
        "lighting/shadow.frag"
        "lighting/composite.vert"
        "lighting/composite.frag")