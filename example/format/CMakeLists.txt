set( EXAMPLE_TARGET format )

target_compile_definitions(
    ${TLV_TARGET}
    PUBLIC
)

add_executable(
    ${EXAMPLE_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)
target_link_libraries( ${EXAMPLE_TARGET} PUBLIC ${TLV_TARGET})
target_compile_options( ${EXAMPLE_TARGET} PRIVATE -Werror -Wall -Wextra -g -O0)
