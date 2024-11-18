# ----------------公共配置--------------------------------

# cmake最低版本
cmake_minimum_required(VERSION 3.28.3)

 # 开启输出编译详细过程的提示
set(CMAKE_VERBOSE_MAKEFILE on)  


# 项目配置
project(lyraLinux LANGUAGES C ASM)

# c语言编译器
set(CMAKE_C_COMPILER, "x86_64-linux-gnu-gcc")

# c语言编译器编译参数
set(CMAKE_C_FLAGS "-g -c -O0 -m32 -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables")
# 汇编器
set(CMAKE_ASM_COMPILER "x86_64-linux-gnu-gcc")
# 汇编器编译参数
set(CMAKE_ASM_FLAGS "-m32 -g")
# 汇编文件后缀
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "s")


# 链接器
set(LINKER_TOOL "x86_64-linux-gnu-ld")

# 文件转换工具，用于将elf文件转换为标准二进制文件
set(OBJCOPY_TOOL "x86_64-linux-gnu-objcopy")
# 反汇编工具
set(OBJDUMP_TOOL "x86_64-linux-gnu-objdump")
# elf文件查看器，用户查看elf文件配置信息
set(READELF_TOOL "x86_64-linux-gnu-readelf")


# ------------------------------boot工程配置------------------------------------
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/boot)

# 设置链接器配置信息
set(CMAKE_C_LINK_EXECUTABLE "${LINKER_TOOL} <OBJECTS> -m elf_i386  -Ttext=0x7c00  --section-start boot_end=0x7dfe -o boot/boot.elf")

# 设置要编译链接的文件列表
file(GLOB C_LIST "boot/*.c" "boot/*.h")
add_executable(${PROJECT_NAME} boot/start.s ${C_LIST})

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND ${OBJCOPY_TOOL} -O binary boot/boot.elf ${CMAKE_SOURCE_DIR}/image/boot.bin
                   COMMAND ${OBJDUMP_TOOL} -x -d -S -m i8086 boot/boot.elf > boot/boot_dis.txt
                   COMMAND ${READELF_TOOL} -a boot/boot.elf > boot/boot_elf.txt
)