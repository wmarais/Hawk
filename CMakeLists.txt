################################################################################
# @file
# @author      Wynand Marais
# @copyright   (c) 2020 Wynand Marais, MIT License
################################################################################
cmake_minimum_required(VERSION 3.10)

project(hawk)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Werror -Wshadow \
  -Wformat=2 -Wformat-truncation -Wundef -fno-common -fstack-usage \
  -Wconversion -ffunction-sections -fdata-sections")


################################################################################
# HAWK KERNEL
################################################################################
set(HAWK_CONF_BUILD_KERNEL ON CACHE BOOL "Enable / disable compilation of the \
  hawk kernel.")

# Check if the kernel should be built.
if(HAWK_CONF_BUILD_KERNEL)

  # Set the CPU typoe to build for.
  set(HAWK_KERNEL_CONF_CPU_TYPE "AMD64" CACHE STRING "The CPU architecture \
    to compile for.")

  # Set all the selectable CPU options.
  set(CACHE HAWK_KERNEL_CONF_CPU_TYPE PROPERTY STRINGS
    AMD64 ARM64)

  # The list of generic header files in the hawk kernel.
  set(HAWK_KERNEL_HEADERS
    include/hawk/kernel/bit_mask.hpp
    include/hawk/kernel/cpu.hpp
    include/hawk/kernel/kernel.hpp
    include/hawk/kernel/memory.hpp
    include/hawk/kernel/panic.hpp
    include/hawk/kernel/register.hpp
    include/hawk/kernel/scheduler.hpp
    include/hawk/kernel/task.hpp
    include/hawk/types.hpp

    include/hawk/kernel/containers/queue.hpp
    include/hawk/kernel/containers/vector.hpp

    include/hawk/kernel/memory/unique_ptr.hpp
  )

  # The list of generic source files in the haw kernel.
  set(HAWK_KERNEL_SOURCES
    source/hawk/kernel/kernel.cpp
    source/hawk/kernel/scheduler.cpp
    source/hawk/kernel/task.cpp
  )

  # Set the architecture / CPU specific source files.
  if(HAWK_KERNEL_CONF_CPU_TYPE STREQUAL "AMD64")
    set(HAWK_KERNEL_CPU_SOURCES source/hawk/kernel/cpu_amd64.cpp)
  endif()

  # Build the kernel.
  #add_executable(hawk_kernel ${HAWK_KERNEL_HEADERS} ${HAWK_KERNEL_SOURCES}
  #                           ${HAWK_KERNEL_CPU_SOURCES})
endif()

#add_subdirectory(comp/bit_mask)

################################################################################
# BOOT LOADER
################################################################################
set(HAWK_CONF_BUILD_BOOT_LOADER ON CACHE BOOL "Enable / disable compilation of \
  the hawk boot loader.")

set(HAWK_CONF_BUILD_BOOTABLE_IMG ON CACHE BOOL "Enable / disable generation \
  of a bootable image.")

# Check if the bootloader should be built.
if(HAWK_CONF_BUILD_BOOT_LOADER)

  set(HAWK_BOOT_LOADDER_HEADERS
    include/hawk/boot/boot_loader.hpp
  )

  set(HAWK_BOOT_LOADER_SOURCES
    source/hawk/boot/boot_loader.cpp
  )

  # Building the shared library object.
  add_library(hawk-boot SHARED ${HAWK_BOOT_LOADDER_HEADERS}
    ${HAWK_BOOT_LOADER_SOURCES})

  # Include the directories where the EFI headers live.
  target_include_directories(hawk-boot PRIVATE
    /usr/include/efi /usr/include/efi/x86_64 )

  target_compile_options(hawk-boot PRIVATE
    -fno-stack-protector          # Stack protection isn't supported by EFI, so
                                  # there's no point in building a binary with
                                  # this feature active.

    -fshort-wchar                 # GCC defines the wchar_t type to be 32 bits
                                  # by default, but EFI requires it to be 16
                                  # bits for 16-bit strings to work correctly.

    -mno-red-zone                 # On x86-64 systems, the red zone is an area
                                  # that follows the stack pointer that can be
                                  # used for temporary variables. The EFI may
                                  # modify this area, though, so it's not safe
                                  # to use, and you must compile EFI binaries
                                  # with this option.
  )

  # -fpic—EFI requires that code be position-independent, hence the use of this
  # option.
  set_target_properties(hawk-boot PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    INTERFACE_POSITION_INDEPENDENT_CODE ON)

  # Link the boot loader. Unfortunately CMake is a bit stupid in not letting you
  # customise the linker etc, so you need to add this as an external command.
  add_custom_command(TARGET hawk-boot POST_BUILD
    COMMAND /usr/bin/ld
    ${CMAKE_BINARY_DIR}/CMakeFiles/hawk-boot.dir/source/hawk/boot/boot_loader.c.o
    /usr/lib/crt0-efi-x86_64.o
    -nostdlib                     # An EFI application should not be linked
                                  # against standard libraries, and this
                                  # argument accomplishes this goal.

    -znocombreloc                 # This argument causes the linker to not
                                  # combine relocation sections.

    -T /usr/lib/elf_x86_64_efi.lds# To create an EFI binary, a non-standard
                                  # linker script must be used, and this option
                                  # tells ld where to find it.

    -shared                       # Even with GNU-EFI's new linker script, ld
                                  # can't create the final executable. Instead,
                                  # it creates a shared library, which is
                                  # subsequently turned into the final binary.

    -Bsymbolic                    # This option causes references to global
                                  # symbols to be bound to the definitions
                                  # within the shared library.
    -L /usr/lib
    -l:libgnuefi.a -l:libefi.a
    -o libhawk-boot.so
  )

  # To create an EFI executable, the Makefile calls the objcopy program, which
  # copies the code needed from the library to create an EFI application.
  add_custom_command(TARGET hawk-boot POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
      -j .text
      -j .sdata
      -j .data
      -j .dynamic
      -j .dynsym
      -j .rel
      -j .rela
      -j .reloc
      --target=efi-app-x86_64
      ${CMAKE_BINARY_DIR}/libhawk-boot.so ${CMAKE_BINARY_DIR}/hawk-boot.efi
  )

  # Check if the bootable image should be built.
  if(HAWK_CONF_BUILD_BOOTABLE_IMG)
    ADD_CUSTOM_TARGET(hawk-bootable-img ALL COMMAND
      ${CMAKE_SOURCE_DIR}/misc/make_boot_img.bash
      ${CMAKE_BINARY_DIR}/hawk-boot.efi
      ${CMAKE_BINARY_DIR}/hawk-boot.img
    )
  endif()
endif()
################################################################################
# DOXYGEN
################################################################################
set(HAWK_BUILD_DOCS ON CACHE BOOL "Build documentation.")

if(HAWK_BUILD_DOCS)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY doxygen)
    set(DOXYGEN_JAVADOC_BANNER YES)
    #doxygen_add_docs(doxygen "${CMAKE_SOURCE_DIR}/include" ALL)
  else()
    set(HAWK_BUILD_DOCS OFF)
    message("Doxygen is required to generate documentation.")
  endif()
endif()

