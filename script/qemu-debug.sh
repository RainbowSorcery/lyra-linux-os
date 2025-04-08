export PROJECT_HOME_PATH=~/os/lyra-linux-os

qemu-system-i386 -daemonize -m 128M -drive file=$PROJECT_HOME_PATH/image/disk1.img,format=raw,index=0,media=disk -s -S -d pcall,page,mmu,cpu_reset,guest_errors,page,trace:ps2_keyboard_set_translation