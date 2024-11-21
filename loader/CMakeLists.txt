project(loader LANGUAGES C)

# ------------------------------loader工程配置------------------------------------
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/loader)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")


# 设置链接器配置信息
set(CMAKE_C_LINK_EXECUTABLE "${LINKER_TOOL} <OBJECTS> -m elf_i386  -Ttext=0x8000  -o loader/loader.elf")

# 设置要编译链接的文件列表
file(GLOB C_LIST "*.c" "*.h" "${CMAKE_SOURCE_DIR}/common/*.h" "${CMAKE_SOURCE_DIR}/common/*.c")
add_executable(${PROJECT_NAME} start.s ${C_LIST})

include_directories(${CMAKE_SOURCE_DIR}/common)

add_custom_command(TARGET loader
                   POST_BUILD
                   COMMAND ${OBJCOPY_TOOL} -O binary ${CMAKE_BINARY_DIR}/loader/loader.elf ${CMAKE_SOURCE_DIR}/image/loader.bin
                   COMMAND ${OBJDUMP_TOOL} -x -d -S -m i8086 ${CMAKE_BINARY_DIR}/loader/loader.elf > ${CMAKE_BINARY_DIR}/loader/loader_dis.txt
                   COMMAND ${READELF_TOOL} -a ${CMAKE_BINARY_DIR}/loader/loader.elf > ${CMAKE_BINARY_DIR}/loader/loader_elf.txt
)