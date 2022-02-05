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
# GCC-Options: 		 https://gcc.gnu.org/onlinedocs/gcc-9.4.0/gcc/C_002b_002b-Dialect-Options.html
	# GCC Debug-Options: https://gcc.gnu.org/onlinedocs/gcc-9.4.0/gcc/Debugging-Options.html#Debugging-Options
	# -Wall 				- activates most of the warnings
	# -Wextra 				- activates even more warnings
	# -pedantic				- Issue all the warnings demanded by strict ISO C and ISO C++
	# -werror				- make every warning a compile error
	# -fstack-protector-all	- protects the stack
	# -Wno-unused-value 	- Warn about unused values
	# -Wformat				- Checks printf matches
	# -Wformat-security 	- Checks for security problems
	# -ggdb 				- Produce debugging information for use by GDB
	# -D-FORTIFY-SOURCE 	- Run-time buffer overflow detection
	add_compile_options(-Wall -Wextra -Werror -fstack-protector-all -Wno-unused-value -Wformat -Wformat-security $<$<CONFIG:Debug>:-ggdb> $<$<CONFIG:Release>:-D_FORTIFY_SOURCE=2>)
endif()


set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
configure_file("config_version.h.in" ${CMAKE_SOURCE_DIR}/include/config_version.h @ONLY)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)
add_subdirectory(examples)