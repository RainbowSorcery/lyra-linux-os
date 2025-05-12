project(kernel LANGUAGES C)

# ------------------------------kernel工程配置------------------------------------
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/kernel)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")


# 设置链接器配置信息
set(CMAKE_C_LINK_EXECUTABLE "${LINKER_TOOL} <OBJECTS> -m elf_i386  -T /home/lyra/os/lyra-linux-os/kernel/kernel.lds -o ${CMAKE_BINARY_DIR}/kernel/kernel.elf")

# 设置要编译链接的文件列表 GLOB_RECURSE递归获取所有.c文件   
file(GLOB_RECURSE C_LIST "*.c" "*.h" "tools/*.c" "init/*.c" "init/*.h" "include/*.c" "include/tools/*.h"  "include/*.h" "cpu/*.c" "cpu/*.h" "include/device/*.h" "include/core/*.h" "cpu/device/*" "cpu/core/*" "${CMAKE_SOURCE_DIR}/common/*.h" "${CMAKE_SOURCE_DIR}/common/*.c")
add_executable(${PROJECT_NAME} init/start.s ${C_LIST})

include_directories(${CMAKE_SOURCE_DIR}/common)

add_custom_command(TARGET kernel
                   POST_BUILD
                   COMMAND ${OBJCOPY_TOOL} -O binary ${CMAKE_BINARY_DIR}/kernel/kernel.elf ${CMAKE_SOURCE_DIR}/image/kernel.bin
                   COMMAND ${OBJDUMP_TOOL} -x -d -S -m i386 ${CMAKE_BINARY_DIR}/kernel/kernel.elf > ${CMAKE_BINARY_DIR}/kernel/kernel_dis.txt
                   COMMAND ${READELF_TOOL} -a ${CMAKE_BINARY_DIR}/kernel/kernel.elf > ${CMAKE_BINARY_DIR}/kernel/kernel_elf.txt
)