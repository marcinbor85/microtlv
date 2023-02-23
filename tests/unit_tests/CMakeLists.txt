create_unit_test( test_unit_tlv_parse
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_tlv_parse.c
    ${TLV_SRC_DIR}/tlv.c
)

create_unit_test( test_unit_tlv_format
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_tlv_format.c
    ${TLV_SRC_DIR}/tlv.c
)

create_unit_test( test_unit_tlv
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_tlv.c
    ${TLV_SRC_DIR}/tlv.c
)
