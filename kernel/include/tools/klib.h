#include "../../../common/types.h"
#include <stdarg.h>

#ifndef KLIB_H
#define KLIB_H

// 字符串复制
void kernel_strcpy(char *dest, const char *src);
void kernel_strcpy_size(char *dest, const char *src, int size);

// 字符串比较
int kernel_strcmp(char *dest, const char *src, int size);

// 字符串长度
int kenerl_strlen(char *dest);

// 内存复制
void kernel_memcpy(void *dest, void *src, int size);

// 内存设置
void kernel_memset(void *dest, unit8_t value, int size);

// 内存比较
int kernel_memcmp(void *dl, void *d2, int size);
void kenerl_sprintf(char *buffer, const char *fmt, ...);
void kenerl_vsprintf(char *buffer, const char *fmt, va_list args);

#endif