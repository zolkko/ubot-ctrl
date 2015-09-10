cmake_minimum_required(VERSION 2.8)
project(ubot-ctrl C CXX)

enable_language(C ASM)

#
# Vendor for MBED
#
set(VENDOR_NAME "STM")
set(VENDOR_FAMILY "STM32F4")
set(VENDOR_CHIP "STM32F407VG")
set(VENDOR_BOARD "DISCO_F407VG")

#
# Linker script filename
#
set(LINKER_SCRIPT "stm32f407_gcc.ld")

#
# Compilation and linking flags
#
set(C_FLAGS "-std=gnu11")
set(CXX_FLAGS "-std=gnu++11 -fno-rtti -fno-exceptions")
set(DEBUG_FLAGS "-g -gstrict-dwarf -Og")
set(NORMAL_FLAGS "-Os")

set(CMAKE_C_FLAGS "${ARCH_FLAGS} ${COMMON_FLAGS} ${C_FLAGS} ${NORMAL_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${ARCH_FLAGS} ${COMMON_FLAGS} ${C_FLAGS} ${DEBUG_FLAGS}")

set(CMAKE_CXX_FLAGS "${ARCH_FLAGS} ${COMMON_FLAGS} ${CXX_FLAGS} ${NORMAL_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${ARCH_FLAGS} ${COMMON_FLAGS} ${CPP_FLAGS} ${DEBUG_FLAGS}")

set(CMAKE_ASM_FLAGS "${ARCH_FLAGS} ${COMMON_FLAGS} ${NORMAL_FLAGS}")
set(CMAKE_ASM_FLAGS_DEBUG "${ARCH_FLAGS} ${COMMON_FLAGS} ${DEBUG_FLAGS}")

#
# App paths
#
set(SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(HW_PATH "${PROJECT_SOURCE_DIR}/hw")

#
# Third party libraries
#
set(THIRD_PARTY "${PROJECT_SOURCE_DIR}/third_party")

set(MBED_PATH "${THIRD_PARTY}/mbed/libraries/mbed")
set(DSP_PATH "${THIRD_PARTY}/mbed/libraries/dsp")
set(CMSIS_PATH "${MBED_PATH}/targets/cmsis")

set(DSP_PID_PATH "${DSP_PATH}/cmsis_dsp/ControllerFunctions")
set(MBED_RTOS_PATH "${THIRD_PARTY}/mbed/libraries/rtos")


#
# Define linker script file
#
set(CMAKE_C_LINK_FLAGS "-specs=\"nano.specs\" -specs=\"nosys.specs\" -Wl,-wrap,main -Wl,-Map,\"${PROJECT_NAME}.map\" -Wl,-T\"${HW_PATH}/${LINKER_SCRIPT}\"")

#
# Define additional pathes where to search
# for header-files.
#
include_directories("${HW_PATH}")
include_directories("${SRC_PATH}")

#
# MBED sdk includes
#
include_directories("${CMSIS_PATH}")
include_directories("${CMSIS_PATH}/TARGET_${VENDOR_NAME}")
include_directories("${CMSIS_PATH}/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}")
include_directories("${CMSIS_PATH}/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/TARGET_${VENDOR_BOARD}")
include_directories("${CMSIS_PATH}/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/TARGET_${VENDOR_CHIP}")

include_directories("${MBED_PATH}/api")
include_directories("${MBED_PATH}/hal")
include_directories("${MBED_PATH}/targets/hal/TARGET_${VENDOR_NAME}")
include_directories("${MBED_PATH}/targets/hal/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}")
include_directories("${MBED_PATH}/targets/hal/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/TARGET_${VENDOR_BOARD}")
include_directories("${MBED_PATH}/targets/hal/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/TARGET_${VENDOR_CHIP}")

#
# MBED RTOS
#
include_directories("${MBED_RTOS_PATH}/rtos")
include_directories("${MBED_RTOS_PATH}/rtx/TARGET_CORTEX_M")

#
# MBED Math
#
include_directories("${DSP_PATH}/cmsis_dsp")

#
# MBED sdk and CMSIS source files
#
aux_source_directory("${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/TARGET_${VENDOR_CHIP}" MBED_CMSIS_VENDOR_SRC)
aux_source_directory("${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}" MBED_CMSIS_SRC)
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_sram.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_can.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_dcmi.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_dcmi_ex.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_eth.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_hcd.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_i2s.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_irda.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_nand.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_nor.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_pccard.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_pcd.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_pcd_ex.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_sd.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_sdram.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_smartcard.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_hal_spdifrx.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_ll_fmc.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_ll_fsmc.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_ll_sdmmc.c")
list(REMOVE_ITEM MBED_CMSIS_SRC "${MBED_PATH}/targets/cmsis/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/stm32f4xx_ll_usb.c")

aux_source_directory("${MBED_PATH}/targets/hal/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}/TARGET_${VENDOR_BOARD}" MBED_HAL_BRD_SRC)
aux_source_directory("${MBED_PATH}/targets/hal/TARGET_${VENDOR_NAME}/TARGET_${VENDOR_FAMILY}" MBED_HAL_SRC)
aux_source_directory("${MBED_PATH}/common" MBED_COMMON_SRC)

#
# MBED rtos source files
#
aux_source_directory("${MBED_RTOS_PATH}/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC" MBED_RTX_HW_SRC)
aux_source_directory("${MBED_RTOS_PATH}/rtx/TARGET_CORTEX_M" MBED_RTX_SRC)
aux_source_directory("${MBED_RTOS_PATH}/rtos" MBED_RTOS_SRC)

#
# DSP library source files
#
aux_source_directory("${DSP_PID_PATH}" MBED_DSP_SRC)
list(REMOVE_ITEM MBED_DSP_SRC "${DSP_PID_PATH}/arm_pid_init_q15.c")
list(REMOVE_ITEM MBED_DSP_SRC "${DSP_PID_PATH}/arm_pid_init_q31.c")
list(REMOVE_ITEM MBED_DSP_SRC "${DSP_PID_PATH}/arm_pid_reset_q15.c")
list(REMOVE_ITEM MBED_DSP_SRC "${DSP_PID_PATH}/arm_pid_reset_q31.c")
list(REMOVE_ITEM MBED_DSP_SRC "${DSP_PID_PATH}/arm_sin_cos_f32.c")
list(REMOVE_ITEM MBED_DSP_SRC "${DSP_PID_PATH}/arm_sin_cos_q31.c")

#
# Search for source files and add them to
# the NAMED lists.
#
aux_source_directory("${HW_PATH}" HW_SRC)
aux_source_directory("${SRC_PATH}" APP_SRC)

#
# Specify compilation target
#

add_executable(${PROJECT_NAME}.elf
    ${MBED_CMSIS_SRC} ${MBED_CMSIS_VENDOR_SRC}
    ${MBED_HAL_SRC} ${MBED_HAL_BRD_SRC} ${MBED_COMMON_SRC}
    # MBED-class rtos (RTX)
    ${MBED_RTX_HW_SRC} ${MBED_RTX_SRC} ${MBED_RTOS_SRC}
    "${MBED_RTOS_PATH}/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/HAL_CM4.S"
    "${MBED_RTOS_PATH}/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/SVC_Table.S"
    # MBED ARM Math
    ${MBED_DSP_SRC}
    # Project source files
    ${HW_SRC} ${APP_SRC})

#
# Libraries to link to. Just in case here we also
# link to stdc++ library.
#
target_link_libraries(${PROJECT_NAME}.elf -lc)
target_link_libraries(${PROJECT_NAME}.elf -lm)
target_link_libraries(${PROJECT_NAME}.elf -lstdc++)

#
# Define additional commands
#
add_custom_target(${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf"
    COMMENT "Generate hex file")

add_custom_target(${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf"
    COMMENT "Generate binary file")

add_custom_target(dump
    COMMAND ${CMAKE_OBJDUMP} -d ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf > ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.S
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf"
    COMMENT "Run dump assembly file")

add_custom_target(size
    COMMAND ${CMAKE_SIZE} -d ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf"
    COMMENT "Print binary code and data size")

add_custom_target(gdbserver
    COMMAND openocd --file "${PROJECT_SOURCE_DIR}/stm32f407.cfg"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf"
    COMMENT "Run openocd gdb server")

add_custom_target(cgdb
    COMMAND cgdb -d ${CMAKE_GDB} -- "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf" -ex "target extended-remote :3333"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf"
    COMMENT "Run cgdb session")

