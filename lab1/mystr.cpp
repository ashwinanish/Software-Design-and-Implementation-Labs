/*
 * ================================================================
 * Filename:    mystr.cpp
 * Description: ECE 412 Lab 1, Part A -- C string utilities,
 *              implemented from scratch. No <cstring> anywhere.
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
    /* TODO: walk to the '\0'. my_strlen("") must be 0. */
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

char *my_strdup(const char *s) {
    /* TODO: malloc exactly the right number of bytes (how many?),
     * copy s including the terminator, return the new buffer.
     * Return NULL if malloc fails. Remember: in C++, malloc's
     * void* must be cast: (char *)malloc(...). */
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
    /* TODO: the strcmp contract. Compare as UNSIGNED CHAR --
     * plain char is signed on mario, and "\xFF" vs "a" will
     * come out with the wrong sign if you compare as char. */
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
    /* TODO: copy src into dst, never writing more than dstsize
     * bytes INCLUDING the terminator. On success return 1.
     * If src does not fit, leave dst a valid empty string and
     * return 0. Think about dstsize == 0 before you write. */
    if (dstsize == 0) 
        return 0;
    while (*src != '\0' && dstsize > 1) {
        *dst++ = *src++;
        dstsize--;
    }
    if (*src != '\0') {
        *dst = '\0';
        return 0;
    }
    *dst = '\0';
    return 1;
}
