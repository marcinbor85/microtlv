set ( TLV_TARGET microtlv )
build_tlv ( ${TLV_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/format )
