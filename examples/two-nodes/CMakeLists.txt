add_executable(opensafety-spdo-exchange
    spdo_exchange.c
    demo_sod.c
    loopback_shnf.c
    link_stubs.c)

target_link_libraries(opensafety-spdo-exchange PRIVATE opensafety_stack)
target_include_directories(opensafety-spdo-exchange PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools/oschecksum/include")
if(WIN32)
    target_link_libraries(opensafety-spdo-exchange PRIVATE ws2_32)
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(opensafety-spdo-exchange PRIVATE -Wl,--gc-sections)
endif()
