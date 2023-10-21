project(kernel)

# basic cmake setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ENABLE_EXPORTS ON)

add_library(
	kernel_lib
	"include/drivers/interrupts/pic.h"
	"include/drivers/memory/memory.h"
	"include/drivers/memory/physical_memory.h"
	"include/drivers/serial/serial.h"
	"src/drivers/interrupts/pic.cpp"
	"src/drivers/memory/memory.cpp"
	"src/drivers/memory/physical_memory.cpp"
	"src/drivers/serial/serial.cpp"
)

target_include_directories(
	kernel_lib
	PUBLIC
	"include"
	"../limine"
)

target_compile_options(
	kernel_lib
	PRIVATE "-Wall"
	PRIVATE "-Wextra"
	PRIVATE "-ffreestanding"
	PRIVATE "-fno-stack-protector"
	PRIVATE "-fno-stack-check"
	PRIVATE "-g"
	PRIVATE "-fno-lto"
	PRIVATE "-fPIE"
	PRIVATE "-m64"
	PRIVATE "-march=x86-64"
	PRIVATE "-mno-80387"
	PRIVATE "-mno-mmx"
	PRIVATE "-mno-sse"
	PRIVATE "-mno-sse2"
	PRIVATE "-mno-red-zone"
)

target_link_options(
	kernel_lib
	PUBLIC "-ffreestanding"
	PUBLIC "-nostdlib"
	PUBLIC "-mcmodel=kernel"
	PUBLIC "-static"
	PUBLIC "-pie"
	PUBLIC "-T${CMAKE_SOURCE_DIR}/slug_kernel/linker.ld"
)

# include the boot image cmake file
include(boot_image.cmake)

target_link_libraries(kernel_lib PUBLIC libc)

add_executable(kernel
	"src/entry.cpp"
)

target_include_directories(
	kernel
	PUBLIC
	"include"
	"../limine"
)

target_compile_options(
	kernel
	PRIVATE "-Wall"
	PRIVATE "-Wextra"
	PRIVATE "-ffreestanding"
	PRIVATE "-fno-stack-protector"
	PRIVATE "-fno-stack-check"
	PRIVATE "-g"
	PRIVATE "-fno-lto"
	PRIVATE "-fPIE"
	PRIVATE "-m64"
	PRIVATE "-march=x86-64"
	PRIVATE "-mno-80387"
	PRIVATE "-mno-mmx"
	PRIVATE "-mno-sse"
	PRIVATE "-mno-sse2"
	PRIVATE "-mno-red-zone"
)

target_link_options(
	kernel
	PUBLIC "-ffreestanding"
	PUBLIC "-nostdlib"
	PUBLIC "-mcmodel=kernel"
	PUBLIC "-static"
	PUBLIC "-pie"
	PUBLIC "-T${CMAKE_SOURCE_DIR}/slug_kernel/linker.ld"
)

target_link_libraries(kernel PUBLIC kernel_lib)
target_link_libraries(kernel PUBLIC libc)
