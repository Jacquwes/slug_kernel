project(libc C)

enable_language(C)

# basic cmake setup
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_library(libc ${SOURCE_FILES} ${HEADER_FILES})

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
