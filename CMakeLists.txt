cmake_minimum_required(VERSION 3.16)
project(http-webserver LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
# https://docs.microsoft.com/de-de/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-160

# GS	- checks buffer security
# Gs	- Controles stack probes
# GT	- Using thread-local storage
# guard:cf	- Adds control flow guard security checks
# RTC1		- Equal to /RTCsu, /RTCs - Stack frame run-time error checking
#							   /RTCu - Variable used without being initialized
add_compile_options(/W4 /GT /GS /Gs /guard:cf $<$<CONFIG:Debug>:/RTC1>)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(InstallRequiredSystemLibraries)
add_definitions(-DNOMINMAX) # to use std::max

elseif(UNIX)
add_compile_options(-Wall -Wextra -pendatic -Werror -fstack-protector)
endif()

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
configure_file("config_version.h.in" ${CMAKE_SOURCE_DIR}/include/config_version.h @ONLY)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)
add_subdirectory(examples)