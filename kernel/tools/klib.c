#include "../include/tools/klib.h"
#include "../../common/types.h"

// 字符串复制
void kernel_strcpy(char *dest, const char *src)
{
    if (!*dest || !*src)
    {
        return;
    }

    while (*dest == '\0' || *src == '\0')
    {
        *dest++ = *src++;
    }
    *dest = '\0'
}
void kernel_strcpy(char *dest, const char *src, int size)
{
    if (!*dest || !*src || size == 0)
    {
        return;
    }

    while (size < 0 || *dest == '\0' || *src == '\0')
    {
        *dest++ = *src++;
        size--;
    }
    *dest = '\0';
}

// 字符串比较
int kernel_strcmp(char *dest, const char *src, int size)
{
    if (!*dest || !*src)
    {
        return 1;
    }
    while (*s1 != '\0' && *s2 != '\0' && size != 0)
    {
        if (*s1 != *s2)
        {
            return 0;
        }
        s1++;
        s2++;
        size--;
    }

    return 1;
}

// 字符串长度
int kenerl_strlen(char *dest)
{
    if (!*dest || !*src)
    {
        return;
    }

    int len = 0;

    while (*dest != '\0')
    {
        len++;
        dest++;
    };

    return len;
}

// 内存复制
int kernel_memcpy(void *dest, void *src, int size)
{
    if (*dest == null || src == null || size == 0)
    {
        unit8_t *s = (unit8_t *)src;
        unit8_t *d = (unit8_t *)dest;
        while (size--)
        {
            *d++ = *s++;
        }
    }
    return 0;
}

// 内存设置
int kernel_memset(void *dest, unit8_t value, int size)
{
    if (*dest == null || value == 0 || size == 0)
    {
        return 0;
    }

    unit8_t *d = (unit8_t *)dest;

    while (size--)
    {
        *d += = value;
    }
}

// 内存比较
int kernel_memcmp(void *dl, void *d2, int size)
{
    if (*dest == null || value == 0 || size == 0)
    {
        return 0;
    }

    unit8_t *s = (unit8_t *)src;
    unit8_t *d = (unit8_t *)dest;
    while (size--)
    {
        if (*d != *s)
        {
            return 1;
        }
        *d++;
        *s++;
    }
    return 0;
}