if (NOT DEFINED TOOLCHAIN_PATH)
	message(FATAL_ERROR "TOOLCHAIN_PATH is not defined")
endif()

set(CMAKE_SYSTEM_NAME Generic)

# Prevent CMake from testing the compiler
set(CMAKE_C_COMPILER_WORKS true)
set(CMAKE_CXX_COMPILER_WORKS true)

# Set the compiler and linker
if (CMAKE_HOST_WIN32)
	set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/x86_64-elf-gcc.exe)
	set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/x86_64-elf-g++.exe)
	set(CMAKE_LINKER ${TOOLCHAIN_PATH}/bin/x86_64-elf-ld.exe)
else()
	set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/x86_64-elf-gcc)
	set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/x86_64-elf-g++)
	set(CMAKE_LINKER ${TOOLCHAIN_PATH}/bin/x86_64-elf-ld)
endif()

# Set compiler flags
set(TARGET_COMPILE_OPTIONS -mcmodel=kernel -mno-red-zone)

set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_CROSSCOMPILING_EMULATOR qemu-system-x86_64;-s;-S)