include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR armv7e-m)

if(TOOLCHAIN_PATH)
    set(TOOLCHAIN  "${TOOLCHAIN_PATH}/arm-none-eabi-" CACHE STRING "")
    set(CMAKE_FIND_ROOT_PATH "${TOOLCHAIN_PATH}")
else(TOOLCHAIN_PATH)
    set(TOOLCHAIN  "arm-none-eabi-" CACHE STRING "")
endif(TOOLCHAIN_PATH)

set(CMAKE_AR "${TOOLCHAIN}ar" CACHE STRING "" FORCE)
set(CMAKE_RANLIB "${TOOLCHAIN}ranlib" CACHE STRING "" FORCE)
set(CMAKE_STRIP "${TOOLCHAIN}strip" CACHE STRING "" FORCE)
set(CMAKE_C_COMPILER "${TOOLCHAIN}gcc" CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER "${TOOLCHAIN}g++" CACHE STRING "" FORCE)
set(CMAKE_OBJCOPY "${TOOLCHAIN}objcopy" CACHE STRING "" FORCE)
set(CMAKE_OBJDUMP "${TOOLCHAIN}objdump" CACHE STRING "" FORCE)
set(CMAKE_NM "${TOOLCHAIN}nm" CACHE STRING "" FORCE)
set(CMAKE_LINKER "${TOOLCHAIN}ld" CACHE STRING "" FORCE)
set(CMAKE_GDB "${TOOLCHAIN}gdb-py" CACHE STRING "" FORCE)
set(CMAKE_SIZE "${TOOLCHAIN}size" CACHE STRING "" FORCE)

CMAKE_FORCE_C_COMPILER("${CMAKE_C_COMPILER}" arm-none-eabi-gcc)
CMAKE_FORCE_CXX_COMPILER("${CMAKE_CXX_COMPILER}" arm-none-eabi-g++)

set(CMAKE_C_ARCHIVE_CREATE "${CMAKE_AR} cr <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_APPEND "${CMAKE_AR} r <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")

set(CMAKE_CXX_ARCHIVE_CREATE ${CMAKE_C_ARCHIVE_CREATE})
set(CMAKE_CXX_ARCHIVE_APPEND ${CMAKE_C_ARCHIVE_APPEND})
set(CMAKE_CXX_ARCHIVE_FINISH ${CMAKE_C_ARCHIVE_FINISH})

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> -Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> -Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group")
SET(CMAKE_ASM_LINK_EXECUTABLE "<CMAKE_ASM_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> -Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group")

set(ARCH_FLAGS "-mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16" CACHE STRING "")
set(COMMON_FLAGS "-ffunction-sections -fdata-sections -Wall" CACHE STRING "")

add_definitions("-DTOOLCHAIN_GCC -DTOOLCHAIN_GCC_ARM -Dgcc")
add_definitions("-D__CORTEX_M4F -D__thumb2__ -DTARGET_ARM7")
add_definitions("-DTARGET_STM32F4 -DTARGET_STM32F407 -DTARGET_F407VG -DSTM32F407xx -DTARGET_STM32F407VG")
# TODO: find a correct way of building mbed-rtos rtx
#   mbed-rtos rtx for some reason does not see __FPU_PRESENT define
#   that is set in cmsis and thus produce code which fails in very stange way
add_definitions("-D__FPU_PRESENT -D__FPU_USED")

# Definition for MBED ARM Math library
add_definitions("-DARM_MATH_CM4")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

