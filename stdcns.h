#ifndef STDCNS_H
#define STDCNS_H

#include <stdint.h>
#include "unistd.h"
#include <stdarg.h>

#define BUF_SIZE 4096
#define ALIGN 64

static inline void putChar(char c) {
    write(STDOUT_FILENO, &c, 1);
}

static inline void putString(const char *str) {
    while (*str) {
        putChar(*str++);
    }
}

static inline void putRaw(const void *data, size_t size) {
    write(STDOUT_FILENO, data, size);
}

static inline void putInt(int num) {
    if (num == 0) {
        putChar('0');
        return;
    }
    
    char buffer[12];
    int i = 0;
    if (num < 0) {
        putChar('-');
        num = -num;
    }
    
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    while (--i >= 0) {
        putChar(buffer[i]);
    }
}

static inline void putUInt(unsigned int num) {
    if (num == 0) {
        putChar('0');
        return;
    }
    
    char buffer[12];
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    while (--i >= 0) {
        putChar(buffer[i]);
    }
}

static inline void putHex(unsigned int num) {
    const char *hexChars = "0123456789ABCDEF";
    
    if (num == 0) {
        putChar('0');
        return;
    }
    
    char buffer[8];
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = hexChars[num % 16];
        num /= 16;
    }
    
    while (--i >= 0) {
        putChar(buffer[i]);
    }
}

static inline void printF(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    char *ptr = (char *)fmt;
    while (*ptr) {
        if (*ptr == '%' && (*(ptr + 1) == 'd' || *(ptr + 1) == 'i')) {
            putInt(va_arg(args, int));
            ptr++;
        } else if (*ptr == '%' && *(ptr + 1) == 'u') {
            putUInt(va_arg(args, unsigned int));
            ptr++;
        } else if (*ptr == '%' && *(ptr + 1) == 'x') {
            putHex(va_arg(args, unsigned int));
            ptr++;
        } else if (*ptr == '%' && *(ptr + 1) == 's') {
            putString(va_arg(args, char *));
            ptr++;
        } else if (*ptr == '%' && *(ptr + 1) == 'c') {
            putChar((char) va_arg(args, int));
            ptr++;
        } else {
            putChar(*ptr);
        }
        ptr++;
    }
    
    va_end(args);
}

#endif // STDCNS_H
