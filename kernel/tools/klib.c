#include "../include/tools/klib.h"
#include "../../common/types.h"

// 字符串复制
void kernel_strcpy(char *dest, const char *src)
{
    if (!*dest || !*src)
    {
        return;
    }

    while (*dest && *src)
    {
        *dest++ = *src++;
    }

    *dest = '\0';
}
void kernel_strcpy_size(char *dest, const char *src, int size)
{
    if (!*dest || !*src || size == 0)
    {
        return;
    }

    char *d = dest;
    const char *s = src;

    while ((size-- > 0) && (*s))
    {
        *d++ = *s++;
    }

    if (size == 0)
    {
        *(d - 1) = '\0';
    }
    else
    {
        *d = '\0';
    }
}

// 字符串比较
int kernel_strcmp(char *s1, const char *s2, int size)
{
    if (!*s1 || !*s2)
    {
        return 1;
    }

    while (*s1 && *s2 && (*s1 == *s2) && size)
    {
        s1++;
        s2++;
    }

    return !((*s1 == '\0') || (*s2 == '\0'));
}

// 字符串长度
int kenerl_strlen(char *s1)
{
    if (!*s1)
    {
        return 0;
    }

    int len = 0;

    while (*s1 != '\0')
    {
        len++;
        s1++;
    };

    return len;
}

// 内存复制
void kernel_memcpy(void *dest, void *src, int size)
{
    if (!dest && !src && size < 0)
    {
        return;
    }

    unit8_t *s = (unit8_t *)src;
    unit8_t *d = (unit8_t *)dest;
    while (size--)
    {
        *d++ = *s++;
    }
}

// 内存设置
void kernel_memset(void *dest, unit8_t value, int size)
{
    if (!dest && size < 0)
    {
        return;
    }
    unit8_t *d = (unit8_t *)dest;
    while (size--)
    {
        *d++ = value;
    }
}

// 内存比较
int kernel_memcmp(void *d1, void *d2, int size)
{
    if (!d1 || !d2 || size > 0)
    {
        return 1;
    }

    unit8_t *p_d1 = (unit8_t *)d1;
    unit8_t *p_d2 = (unit8_t *)d2;

    while (size--)
    {
        if (*p_d1++ != *p_d2++)
        {
            return 1;
        }
    }

    return 0;
}

void kenerl_sprintf(char *buffer, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    kenerl_vsprintf(buffer, fmt, args);
    va_end(args);
}

void kernel_itao(char *buffer, int num, int base)
{
    char *p = buffer;
    static const char *num2ch = {"0123456789ABCDEF"};

    if (num < 0)
    {
        *p++ = '-';
        // 将负数乘-1转为正数
        num *= -1;
    }

    int len = 0;
    char strBuffer[1024] = {};
    while (num != 0)
    {
        uint16_t value = num % base;
        char str = num2ch[value];
        strBuffer[len] = str;
        num /= base;
        len++;
    }

    for (int i = len; i > 0; i--)
    {
        *p++ = strBuffer[i - 1];
    }
}

void kenerl_vsprintf(char *buffer, const char *fmt, va_list args)
{

    enum
    {
        NORMAL,
        READ_PMT
    } state = NORMAL;

    // 当前写的位置
    char *curr = buffer;

    char ch;
    while (ch = *fmt++)
    {
        switch (state)
        {
        case NORMAL:
            // 如果字符串是以%开头的 那么切换状态机
            if (ch == '%')
            {
                state = READ_PMT;
            }
            else
            {
                *curr++ = ch;
            }
            break;
        case READ_PMT:
            if (ch == 's')
            {
                char *str = va_arg(args, char *);
                int len = kenerl_strlen(str);
                while (len--)
                {
                    *curr++ = *str++;
                }
            }
            else if (ch == 'd')
            {
                int num = va_arg(args, int);
                kernel_itao(curr, num, 10);
                kenerl_strlen(curr);
            }
            else if (ch == 'x')
            {
                int num = va_arg(args, int);
                kernel_itao(curr, num, 16);
                kenerl_strlen(curr);
            }
            else if (ch == 'c')
            {
                char *str = va_arg(args, char *);
                *curr++ = *str++;
            }

            state = NORMAL;
            break;
        }
    }
}