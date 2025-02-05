#ifndef TIME_H
#define TIME_H

// 时钟源
#define RIT_OSC_FREQ 1193182
#define PIT_COMMAND_MODE_PORT 0x43
#define PIT_COMMAND_DATA_PORT 0x40

#define PIT_COMMAND_CHANNEL_0 (0 << 6)
#define PIT_COMMAND_ACCESS_MODE (3 << 4)
#define PIT_COMMAND_OPER_MODE (3 << 1)

// 每10毫秒中断一次时钟中断
#define OS_TICKS_MS 10

// 初始化定时器
void time_init();
// 定时器解析函数
void exception_handler_time();

#endif // CPU_INSTR_H