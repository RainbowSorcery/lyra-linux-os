## 保护模式

### 启动流程

- 通电

  按下开机键，CPU会从ROM里读取BIOS系统并加载到内存中。

- 执行BIOS程序

  加载完成后会执行BIOS程序

- BIOS自检

  在BIOS执行过程中会对计算机硬件进行自检避免运行出错

- 运行引导代码

  之后会读取硬盘扇区末尾为0x55aa的扇区并加载到内存0x7c00中，一个扇区大小为512字节。我们的操作系统512字节肯定是不够用的，所以我们需要利用引导代码读取我们的操作系统并加载到内存中。

- 进入操作系统

  执行我们的操作系统的代码。

![image-20241014212426778](./assets/image-20241014212426778.png)

.org用于指定接下来代码数据应该设置到什么位置，.org 0x1fe开始写入数据，写入数据内容为0x55, 0xaa

0x1fe的十进制为510，这样就将一个扇区的末尾设置成0x55aa，这样BIOS就会将这个引导扇区的代码加载到0x7c00中。

汇编会从_start:开始执行

```assembly

	#include "os.h"

	// 声明本地以下符号是全局的，在其它源文件中可以访问
	.global _start

	// 指定以下的代码生成16位的机器指令，这样才能在启动时的实模式下运行
  	.code16

	// 以下是代码区
 	.text
_start: 
	jmp .
	.org 0x1fe
	.byte 0x55, 0xaa
```



### x86处理器编程模型

通用寄存器大多数都可以混着用的

通用寄存器: 

**EAX**：累加器寄存器，用于算术和逻辑运算。

**EBX**：基址寄存器，通常用于基址寻址。

**ECX**：计数器寄存器，通常用于循环计数。

**EDX**：数据寄存器，用于 I/O 操作和扩展乘法除法运算。

**ESI**：源索引寄存器，通常用于指向数据源。

**EDI**：目的索引寄存器，通常用于指向数据目的。

**EBP**：基址指针寄存器，通常用于栈操作。

**ESP**：栈指针寄存器，指向栈的顶部。

段寄存器寻址: 段基址 << 4 + 偏移	

段寄存器: 

1. **CS（代码段寄存器，Code Segment Register）**

- **功能**：CS 存储当前代码段的基址，指向正在执行的代码段。所有指令都是通过 CS 来定位其在内存中的物理地址。
- **用途**：当 CPU 执行指令时，CS 中的值与 EIP（指令指针寄存器）配合使用来确定下一条指令的物理地址。

2. **DS（数据段寄存器，Data Segment Register）**

- **功能**：DS 存储当前数据段的基址，指向数据存储的内存段。
- **用途**：所有数据操作，除非另有指定，默认都通过 DS 来定位数据的物理地址。

3. **SS（堆栈段寄存器，Stack Segment Register）**

- **功能**：SS 指向堆栈段的基址，管理堆栈操作。
- **用途**：堆栈操作（如 PUSH 和 POP 操作）使用 SS 和 ESP（栈指针寄存器）来确定堆栈在内存中的位置。

4. **ES（额外段寄存器，Extra Segment Register）**

- **功能**：ES 是一个额外的段寄存器，通常在字符串操作指令中与 EDI（目的索引寄存器）配合使用。
- **用途**：在执行字符串复制指令（如 `MOVS`）时，ES 通常用于指向目标内存段。

5. **FS（附加段寄存器，Additional Segment Register）**

- **功能**：FS 是一个额外的段寄存器，通常用于存储特殊数据结构的基址，如线程本地存储（Thread Local Storage, TLS）。
- **用途**：FS 可用于高级操作系统结构，或是为了提供额外的段访问。

6. **GS（全局段寄存器，Global Segment Register）**

- **功能**：GS 是另一个附加段寄存器，类似于 FS，但通常在不同场合下使用。
- **用途**：GS 也可以用于存储特殊数据结构的基址，或为操作系统提供扩展段访问。

![image-20241014230954018](./assets/image-20241014230954018.png)

在保护模式案例中64K内存空间足够使用了，所以所有段寄存器的值都设置成0即可。





### 加载其余部分

调用BIOS中断0x13就可以读取硬盘数据到内存中

**AH**: 设置为 `02h` 来读取扇区。

**AL**: 要读取的扇区数量（最多可以为 `FFh`，即 255）。

**CH**: 磁头的柱面号（Cylinder）。

**CL**: 扇区号（Sector），范围从 `01h` 到 `3Fh`。

**DH**: 磁头号（Head）。

**BX**: 指向存储数据的缓冲区的段:偏移地址。

```assembly

read_disk_all:
    ; 从0x7e00开始读, 0x7c00 + 512引导扇区 = 0x7e00
	mov $0x7e00, %bx
	mov $0x02	, %cx
	; 读取64个扇区
	mov $0x240, %ax
	mov $0x80, %dx
	int $0x13
	jc read_disk_all
```



### 保护模式

实模式是直接根据数据段和代码段寄存器来访问指定内存区域，所有的程序都可以修改段寄存器来访问内存中的任意区域，这样会使系统安全性降低，没有任何限制。

在保护模式中，程序访问内存数据时会校验权限以及是否越界，这样就会过滤掉恶意代码对计算机操作系统进行修改。

保护模式编程模型如下图所示:

![image-20241022012505445](./assets/image-20241022012505445.png)

在实模式中寻址方式是根据段基地址:偏移来进行寻址的，而保护模式是根据段寄存器中保存的段下标找到GDT段信息，在段信息中有段的起始地址以及权限信息，然后根据段信息再进行寻址访问的。

![image-20241022013458388](./assets/image-20241022013458388.png)



![image-20241022012147131](./assets/image-20241022012147131.png)

16位段属性，access_right的高四位表示扩展访问权限：

- G: 段界限为20位。如果为0时以字节方式寻址，段的寻址范围1B-1M; 如果为1则是以4k为单位寻址，段的寻址访问为4k-4G。一般32位模式设置成1，很明显，1M根本不够日常使用的。
- D/B: 默认操作数大小, 为1表示按32位寻址，为0表示16位寻址，16位一般用于80286的CPU中，其他基本都是使用32位的寻址方式。
- L: 64位代码标识，保留给64位处理器使用。
- AVL: 自定义标志位，用于开发人员自定义或扩展。
低8位:
- P: 如果为1表示该段存在，可以正常访问，如果为0表示该段不存在，强行访问会出现异常。
- DPL: 特权级, 分为4个特权级, 0 - 3,  0权限为最高，3为最低。
- S: 0表示系统段，1表示代码段。
- TYPE: 段的访问权限。
  x = 0时不可执行; x = 1时可执行; 
  e = 0时向高地址增长;
  e = 1时向低地址增长; w = 0时只读; 
  w = 1时可读可写;
  a = 0时段最近未被访问; 
  a = 1时段最近被访问过。
  c = 0时非特权依存段, 非特权依存段只能调用相同特权级别相同的代码段;
  c = 1时特权依存段, 特权依存段可以调用低权限代码段;
  r = 0时不可断;
  r = 1时可读;
  当S位等于1时, 且为数据段时TYPE四位表示X, E, W, A。当S等于1时，且为代码段时，TYPE四位表示X, C, R, A;
  当S位等于0时表示系统描述符，共有以下几种取值范围:
  TYPE = 0, 8, 10, 13：保留字段
  TYPE = 1：可用的 16 位任务状态段(TSS)描述符
  TYPE = 2：LDT 的段描述符
  TYPE = 3：正忙的 16 位 TSS 描述符
  TYPE = 4：16 位调用门
  TYPE = 5：任务门
  TYPE = 6：16 位中断门
  TYPE = 7：16 位陷阱门
  TYPE = 9：可用的 32 位 TSS 描述符
  TYPE = 11：正忙的 32 位 TSS 描述符
  TYPE = 12：32 位调用门
  TYPE = 14：32 位中断门
  TYPE = 15：32 位陷阱门





## 磁盘映像文件

## RAW

`RAW` 格式广义上指的是一种没有文件系统结构的原始数据存储格式，常用于直接读取和写入磁盘设备的物理扇区。这种格式非常适合数据恢复、虚拟化、嵌入式系统等场景。下面是 `RAW` 格式的详解，包括其原理、应用场景和使用方法。

创建RAW磁盘映像文件

qemu-img create -f raw /home/lyra/os/lyra-linux/image/disk1.img 100M

创建一个名称为disk1的100M大小的虚拟磁盘文件。



​	

## 操作系统运行流程

![image-20241103103004789](./assets/image-20241103103004789.png)

boot: 读取磁盘后加载loader到内存中。

loader: 操作系统的加载以及保护模式等的设置。

kernel: 操作系统内核执行。



## loader

```assembly
    #include "boot.h"
    .code16
    .text
    .global _start
    .extern boot_entry

_start:
    mov $0, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov 0x7c00, %esp
    ; 加载loader到0x8000地址中，之前执行0x8000没有办法设置寄存器的原因是没有被加载到内存中，所以一直在执行空指令，能设置寄存器才怪了
    mov $0x8000, %bx
    mov $0x2, %ah
    mov $0x2, %cx
    mov $64, %al
    mov $0x0080, %dx
    int $0x13

    jmp boot_entry

.section boot_end, "ax"
boot_sig: .byte 0x55, 0xaa
```





## 系统编译流程

编译流程:
1. 使用x86_64-linux-gnu-gcc命令将汇编文件编译输出.o文件，o文件是利用汇编器生成的目标程序。.o文件可以与其他.o文件链接一起最终输出可执行程序，例如汇编程序生成的.o文件与c语言生成的.o文件，使用链接器将它们链接在一起就能实现c语言调用汇编代码，汇编代码调用c语言了。
例：
```bash
 # 将汇编程序输出为目标程序
 x86_64-linux-gnu-gcc -m32 -g -c start.S -o start.o
```
2. 使用 x86_64-linux-gnu-ld链接器将目标程序进行链接并输出elf文件。-m参数用于指定输出文件指令架构,i386是Intel 32位; -Text表示将代码放置到0x7c00地址处;--section-start表示设置段的内存起始位置，与代码中的boot_end是保持一致的，a表示段会被分配到程序内存空间中，x表示代码可以被执行的。如下图所示:

   ![image-20241118010541991](./assets/image-20241118010541991.png)

```bash
x86_64-linux-gnu-ld start.o -m elf_i386  -Ttext=0x7c00  --section-start boot_end=0x7dfe -o ./boot.elf
```

3. elf文件虽然是二进制文件，但是它是一种结构化的二进制文件，包含了许多程序头、符号表信息，需要将elf文件转换为纯粹二进制文件才能被计算机进行加载。使用objcopy命令可以完成这个操作。

```bash
objcopy -O binary boot.elf  boot.bin
```

其他工具:

1. objdump: 将二进制文件进行反汇编。-x: 显示目标文件头信息; -d: 机器码与汇编文件对应操作; -S反汇编的同时输出源代码信息; -m 指定指令集.

```bash
objdump -x -d -S -m i8086 boot.elf 
```

2. x86_64-linux-gnu-readelf：用于查看elf文件的符号信息的,

使用这个命令可以知道文件的的相关信息，例如我们之前设置的_start段的地址0x7c00就可以利用readelf命令查询到。

![image-20241118011811817](./assets/image-20241118011811817.png)

```bash
x86_64-linux-gnu-readelf -a boot.elf 
```



## cmake配置

### 公共配置

用于配置项目的全局配置，例如cmake的版本，使用的编译器链接器等等

```cmake
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
```

### boot

boot项目的配置信息，例如编译后的结果要加载到内存的什么位置，链接器的参数什么，要编译哪些文件等等。

```cmake
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
```

