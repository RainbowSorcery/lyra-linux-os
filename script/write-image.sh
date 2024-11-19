export DISK1_NAME=/home/lyra/os/lyra-linux/image/disk1.img
export BOOT_FILE_NAME=/home/lyra/os/lyra-linux/image/boot.bin
export LOADER_FILE_NAME=/home/lyra/os/lyra-linux/image/loader.bin

# 写boot区，定位到磁盘开头，写1个块：512字节
dd if=$BOOT_FILE_NAME of=$DISK1_NAME bs=512 conv=notrunc count=1

# 写loader区，定位到磁盘第2个块，写1个块：512字节
dd if=$LOADER_FILE_NAME of=$DISK1_NAME bs=512 conv=notrunc seek=1
