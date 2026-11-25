cmake_minimum_required(VERSION 3.16)
project(openSAFETYPlayground VERSION 0.1.0 LANGUAGES C)

include(CheckIncludeFile)
include(CheckFunctionExists)

set(openSAFETY_VERSION 1)
set(openSAFETY_CERTIFICATION 4)
set(openSAFETY_VERSION_RELEASE "1.4.0")
set(openSAFETY_VERSION_NAME "'Playground build'")

check_include_file("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_file("winsock2.h" HAVE_WINSOCK2_H)
check_include_file("netinet/in.h" HAVE_NETINET_IN_H)
check_include_file("arpa/inet.h" HAVE_ARPA_INET_H)
check_include_file("arpa/nameser.h" HAVE_ARPA_NAMESER_H)
check_include_file("sys/param.h" HAVE_SYS_PARAM_H)
check_include_file("sys/types.h" HAVE_SYS_TYPES_H)
check_function_exists(bzero HAVE_BZERO)
check_function_exists(inet_pton HAVE_INET_PTON)
check_function_exists(snprintf HAVE_SNPRINTF)
check_function_exists(strdup HAVE_STRDUP)
check_function_exists(vsnprintf HAVE_VSNPRINTF)

if(WIN32)
    set(TARGET_WINDOWS 1)
    set(TARGET_GENERIC 1)
endif()

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${generated_dir}")
configure_file(src/config.h.in "${generated_dir}/config.h")

file(GLOB_RECURSE opensafety_sn_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/eplssrc/SN/*.c")
file(GLOB_RECURSE opensafety_scm_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/eplssrc/SCM/*.c")

add_library(opensafety_stack STATIC
    ${opensafety_sn_sources}
    ${opensafety_scm_sources})

target_include_directories(opensafety_stack PUBLIC
    "${generated_dir}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/eplssrc/SN"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/eplssrc/SCM"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/eplssrc/contrib")

target_compile_features(opensafety_stack PUBLIC c_std_99)

if(WIN32)
    target_compile_definitions(opensafety_stack PUBLIC _WIN32_WINNT=0x0601)
endif()

