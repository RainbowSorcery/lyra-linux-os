// log.h
#ifndef LOG_H
#define LOG_H

extern void log_init();
extern int is_transmit_empty();
extern void log_printf(const char *format, ...);

#endif // LOG_H