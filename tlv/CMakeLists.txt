file(
    GLOB
    SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tlv.c
)

set(
    TLV_SRC_FILES
    ${SRC_FILES}
    PARENT_SCOPE
)

set(
    TLV_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)

set(
    TLV_SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)

function( build_tlv TARGET )
    add_library(
        ${TARGET}
        STATIC
        ${TLV_SRC_FILES}
    )

    target_include_directories(
        ${TARGET}
        PUBLIC
        ${ARGN}
        ${TLV_INC_DIRS}
    )

    target_compile_options(
        ${TARGET}
        PRIVATE
        -Werror -Wall -Wextra  -g -O0
    )
endfunction()
