/*
 * ================================================================
 * Filename:    mystr.cpp
 * Description: ECE 412 Lab 2 -- C string utilities. This is the
 *              same file you wrote for Lab 1. Bring your Lab 1
 *              implementation over, or reimplement here.
 *
 *              Only my_strdup is strictly required by db.cpp for
 *              this lab. The others are here so your `main.cpp`
 *              can keep using the same toolkit you built last week.
 * Created:     TODO
 * EID:         TODO
 * Email:       TODO
 * Author:      TODO
 * Provenance:  I certify that all code contained herein is mine
 *              alone except where otherwise noted.
 * ================================================================
 */

#include <stdlib.h>    /* malloc, free -- NOT <cstring>! */

#include "mystr.h"

size_t my_strlen(const char *s) {
    /* TODO (from Lab 1). */
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

char *my_strdup(const char *s) {
    /* TODO (from Lab 1). db.cpp depends on this: DB_add's deep copy
     * of the caller's `name` is a my_strdup, and the trap of storing
     * the caller's `const char *` directly is the same dangling-pointer
     * bug we discussed last week. */
    size_t len = my_strlen(s);
    char *str = (char *)malloc(len + 1);
    if (str == NULL) {
        return NULL;
    }
    for (size_t i = 0; i <= len; i++) {
        str[i] = s[i];
    }
    return str;
}

int my_strcmp(const char *a, const char *b) {
    /* TODO (from Lab 1). */
    unsigned char *ua = (unsigned char *)a;
    unsigned char *ub = (unsigned char *)b;

    while (*ua != '\0' && *ub != '\0') {
        if (*ua != *ub) {
            return (int)(*ua) - (int)(*ub);
        }
        ua++;
        ub++;
    }

    if (*ua == '\0' && *ub == '\0') {
        return 0;
    }
    if (*ua == '\0') {
        return -1;
    }
    return 1;
}

int my_strcpy_safe(char *dst, size_t dstsize, const char *src) {
    /* TODO (from Lab 1). */
    if (dstsize == 0) 
        return 0;
    size_t i = 0;
    while (src[i] != '\0' && i < dstsize - 1) {
        dst[i] = src[i];
        i++;
    }
    if (src[i] != '\0') {
        dst[0] = '\0';
        return 0;
    }
    dst[i] = '\0';
    return 1;
}
