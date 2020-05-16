cmake_minimum_required(VERSION 3.10)

project(hawk)



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
    include/hawk/kernel/panic.hpp
    include/hawk/kernel/queue.hpp
    include/hawk/kernel/register.hpp
    include/hawk/kernel/scheduler.hpp
    include/hawk/kernel/task.hpp
    include/hawk/types.hpp
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
  add_executable(hawk_kernel ${HAWK_KERNEL_HEADERS} ${HAWK_KERNEL_SOURCES}
                             ${HAWK_KERNEL_CPU_SOURCES})
endif()


add_subdirectory(/home/wmadmin/Hawk/comp/bit_mask)


################################################################################
# DOXYGEN
################################################################################
set(HAWK_BUILD_DOCS ON CACHE BOOL "Build documentation.")

if(HAWK_BUILD_DOCS)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY doxygen)
    set(DOXYGEN_JAVADOC_BANNER YES)
    doxygen_add_docs(doxygen "${CMAKE_SOURCE_DIR}/include" ALL)
  else()
    set(HAWK_BUILD_DOCS OFF)
    message("Doxygen is required to generate documentation.")
  endif()
endif()

