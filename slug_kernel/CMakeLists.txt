project(kernel)

# basic cmake setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ENABLE_EXPORTS ON)

add_library(
	kernel_lib
	"include/arch/x86_64/gdt.h"
	"include/drivers/interrupts/pic.h"
	"include/drivers/memory/memory.h"
	"include/drivers/memory/physical_memory.h"
	"include/drivers/serial/serial.h"
	"src/arch/x86_64/gdt.cpp"
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

set(ISO_DIR ${CMAKE_CURRENT_BINARY_DIR}/iso)
set(LIMINE_DIR ${CMAKE_SOURCE_DIR}/limine)

# convenience target to build the kernel image
if (CMAKE_HOST_WIN32)
	add_custom_target(
		bootable_image
		COMMAND powershell mkdir -Force ${ISO_DIR}
		COMMAND powershell cp ${CMAKE_CURRENT_BINARY_DIR}/kernel ${ISO_DIR}/
		COMMAND powershell cp ${LIMINE_DIR}/limine-bios.sys ${ISO_DIR}/
	 	COMMAND powershell cp ${LIMINE_DIR}/limine-bios-cd.bin ${ISO_DIR}/
	 	COMMAND powershell cp ${CMAKE_SOURCE_DIR}/slug_kernel/limine.cfg ${ISO_DIR}/
		COMMAND powershell cp ${LIMINE_DIR}/limine-uefi-cd.bin ${ISO_DIR}/
		COMMAND powershell mkdir -Force ${ISO_DIR}/EFI/BOOT
		COMMAND powershell cp ${LIMINE_DIR}/BOOTX64.EFI ${ISO_DIR}/EFI/BOOT/
		COMMAND powershell cp ${LIMINE_DIR}/BOOTIA32.EFI ${ISO_DIR}/EFI/BOOT/
		COMMAND pwd
		COMMAND powershell xorriso -as mkisofs -b limine-bios-cd.bin
	 		-no-emul-boot -boot-load-size 4 -boot-info-table
	 		--efi-boot limine-uefi-cd.bin
	 		-efi-boot-part --efi-boot-image --protective-msdos-label
	 		iso -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.iso
		COMMAND powershell ${LIMINE_DIR}/limine.exe bios-install ${CMAKE_CURRENT_BINARY_DIR}/kernel.iso
		COMMAND powershell rm -r ${ISO_DIR}
		DEPENDS kernel
		VERBATIM
	)
else()
	add_custom_target(
		bootable_image
		COMMAND rm -rf ${ISO_DIR}
		COMMAND mkdir -p ${ISO_DIR}
		COMMAND cp -v $<TARGET_FILE:kernel> 
	 		${CMAKE_SOURCE_DIR}/slug_kernel/limine.cfg ${LIMINE_DIR}/limine-bios.sys ${LIMINE_DIR}/limine-bios-cd.bin
			${LIMINE_DIR}/limine-uefi-cd.bin ${ISO_DIR}/
		COMMAND mkdir -p ${ISO_DIR}/EFI/BOOT
		COMMAND cp -v ${LIMINE_DIR}/BOOTX64.EFI ${ISO_DIR}/EFI/BOOT/
		COMMAND cp -v ${LIMINE_DIR}/BOOTIA32.EFI ${ISO_DIR}/EFI/BOOT/
		COMMAND xorriso -as mkisofs -b limine-bios-cd.bin
	 		-no-emul-boot -boot-load-size 4 -boot-info-table
	 		--efi-boot limine-uefi-cd.bin
	 		-efi-boot-part --efi-boot-image --protective-msdos-label
	 		${ISO_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.iso
		COMMAND ${LIMINE_DIR}/limine bios-install ${CMAKE_CURRENT_BINARY_DIR}/kernel.iso
		COMMAND rm -rf ${ISO_DIR}
		DEPENDS kernel
		VERBATIM
	)
endif()

add_custom_target(
	qemu_kernel
	COMMAND  start /B qemu-system-x86_64 -s -S -cdrom ${CMAKE_CURRENT_BINARY_DIR}/kernel.iso
	DEPENDS bootable_image
	VERBATIM
)