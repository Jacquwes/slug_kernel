project(libc C)

enable_language(C)

add_library(
	libc
	"include/stddef.h"
	"include/stdint.h"
	"include/stdio.h"
	"include/stdlib.h"
	"include/string.h"
	"src/stdio/printf.cpp"
	"src/stdlib/memory.cpp"
	"src/string/examination.cpp"
	"src/string/array_manipulation.cpp")

set_target_properties(libc PROPERTIES LINKER_LANGUAGE C)

target_include_directories(
	libc
	PUBLIC
	"include"
)

target_compile_options(
	libc
	PRIVATE "-Wall"
	PRIVATE "-Wextra"
	PRIVATE "-ffreestanding"
	PRIVATE "-fno-stack-protector"
	PRIVATE "-fno-stack-check"
	PRIVATE "-fno-lto"
	PRIVATE	"-g"
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
	libc
	PUBLIC "-ffreestanding"
	PUBLIC "-nostdlib"
	PUBLIC "-mcmodel=kernel"
	PUBLIC "-static"
	PUBLIC "-pie"
)

target_link_libraries(libc PUBLIC kernel_lib)
