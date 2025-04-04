export PROJECT_HOME_PATH=/home/lyra/os/lyra-linux-os

export DISK1_NAME=$PROJECT_HOME_PATH/image/disk1.img
export BOOT_FILE_NAME=$PROJECT_HOME_PATH/image/boot.bin
export LOADER_FILE_NAME=$PROJECT_HOME_PATH/image/loader.bin
export KERNEL_FILE_NAME=$PROJECT_HOME_PATH/image/kernel.bin



# 写boot区，定位到磁盘开头，写1个块：512字节
echo "写入boot分区......"
dd if=$BOOT_FILE_NAME of=$DISK1_NAME bs=512 conv=notrunc count=1

# 写loader区，定位到磁盘第2个块，写1个块：512字节
echo "写入loader分区......"
dd if=$LOADER_FILE_NAME of=$DISK1_NAME bs=512 conv=notrunc seek=1

# 写kernel分区，100号扇区之后的内容就是kernel的数据内容
echo "写入kernel分区......"
dd if=$KERNEL_FILE_NAME of=$DISK1_NAME bs=512 conv=notrunc seek=100
