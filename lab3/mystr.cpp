/* ------------------------------------------------------------------ *
 * mystr.cpp -- my_strdup, from Lab 2.
 * ------------------------------------------------------------------ */
#include <cstdlib>
#include <cstring>

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
