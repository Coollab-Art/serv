cmake_minimum_required(VERSION 3.20)

add_library(serv)
add_library(serv::serv ALIAS serv)
target_compile_features(serv PRIVATE cxx_std_17)

if (SERV_ENABLE_WARNINGS_AS_ERRORS)
    message("-- [serv] Enabling warnings as errors for serv")
    if(MSVC)
        target_compile_options(serv PRIVATE /WX /W4)
    else()
        target_compile_options(serv PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wno-unknown-pragmas)
    endif()
else()
    message("-- [serv] Not using warnings as errors for serv")
endif()

# ---Add source files---
target_include_directories(serv PUBLIC
    include)
target_include_directories(serv PRIVATE
    lib/civetweb/include)
target_sources(serv PRIVATE
    src/serv.cpp
    lib/civetweb/src/civetweb.c
    lib/civetweb/src/CivetServer.cpp
)

# ---CivetWeb config---
target_compile_definitions(serv PRIVATE NO_SSL)