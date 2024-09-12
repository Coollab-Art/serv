cmake_minimum_required(VERSION 3.20)

set(WARNINGS_AS_ERRORS_FOR_SERV OFF CACHE BOOL "ON iff you want to treat warnings as errors")

add_library(serv)
add_library(serv::serv ALIAS serv)
target_compile_features(serv PRIVATE cxx_std_17)

# Set warning level
if(MSVC)
    target_compile_options(serv PRIVATE /W4)
else()
    target_compile_options(serv PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wno-unknown-pragmas)
endif()

# Maybe enable warnings as errors
if(WARNINGS_AS_ERRORS_FOR_SERV)
    if(MSVC)
        target_compile_options(serv PRIVATE /WX)
    else()
        target_compile_options(serv PRIVATE -Werror)
    endif()
endif()

# ---Add source files---
if(WARNINGS_AS_ERRORS_FOR_SERV)
    target_include_directories(serv PUBLIC include)
else()
    target_include_directories(serv SYSTEM PUBLIC include)
endif()

target_sources(serv PRIVATE
    src/serv.cpp
)

# ---CivetWeb---
add_library(civetweb)
target_include_directories(civetweb SYSTEM PUBLIC lib/civetweb/include)
target_sources(civetweb PRIVATE
    lib/civetweb/src/civetweb.c
    lib/civetweb/src/CivetServer.cpp
)
target_compile_definitions(civetweb PRIVATE NO_SSL)
target_compile_options(civetweb PRIVATE -w)
target_link_libraries(serv PUBLIC civetweb)
install(FILES "lib/civetweb/LICENSE.md" DESTINATION "license/civetweb")