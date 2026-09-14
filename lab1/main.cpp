/*
 * ================================================================
 * Filename:    main.cpp
 * Description: ECE 412 Lab 1, Part C -- driver. Exercises Parts A
 *              and B, including every failure path you can reach.
 *              This whole program must be valgrind-clean.
 * Created:     TODO
 * EID:         TODO
 * Email:       TODO
 * Author:      TODO
 * Provenance:  I certify that all code contained herein is mine
 *              alone except where otherwise noted.
 * ================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "intarray.h"
#include "mystr.h"

int main(void) {
    /* ---- 1. Growth demo ------------------------------------------------
     * TODO: push at least 40 values so the array doubles at least three
     * times (4 -> 8 -> 16 -> 32 -> 64). After EACH push print size, cap,
     * and the buffer address, so the doublings -- and any address change --
     * are visible:
     *
     *     printf("push %2d: size=%2zu cap=%2zu data=%p\n",
     *            v, ia_size(&a), ia_capacity(&a), (void *)a.data);
     *
     * Circle in your output where the address changed (or did not) and
     * connect it to your WRITEUP's realloc-vs-manual answer.
     */
     for(int i = 0; i < 40; i++) {
        IntArray a;
        ia_init(&a);
        if (!ia_push_back(&a, i)) {
            printf("Failed to push %d\n", i);
            ia_destroy(&a);
            return 1;
        }
        printf("push %2d: size=%2zu cap=%2zu data=%p\n",
               i, ia_size(&a), ia_capacity(&a), (void *)a.data);
        ia_destroy(&a);
    }

    /* ---- 2. Names: the two-level ownership -----------------------------
     * TODO: build an array of my_strdup'd names (a char** you malloc, or a
     * fixed char *names[N]), sort it with my_strcmp (your own insertion
     * sort is fine), print the sorted names -- then free EVERY string AND
     * (if malloc'd) the array of pointers. Two levels, two kinds of free.
     */
    
    char **names = (char **)malloc(5 * sizeof(char *));
    if (names == NULL) {
        printf("Failed to allocate names array\n");
        return 1;
    }
    const char *input_names[] = {"Charlie", "Alice", "Eve", "Bob", "David"};
    size_t size = sizeof(input_names) / sizeof(input_names[0]);
    for (size_t i = 0; i < size; i++) {
        names[i] = my_strdup(input_names[i]);
        if (names[i] == NULL) {
            printf("Failed to duplicate name: %s\n", input_names[i]);
            for (size_t k = 0; k < i; k++) {
                free(names[k]);
            }
            free(names);
            return 1;
        }
    }
    for (size_t i = 0; i < size; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (my_strcmp(names[i], names[j]) > 0) {
                char *temp = names[i];
                names[i] = names[j];
                names[j] = temp;
            }
        }
    }
    printf("Sorted names:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%s\n", names[i]);
    }
    for (size_t i = 0; i < size; i++) {
        free(names[i]);
    }
    free(names);

    /* ---- 3. Failure paths ----------------------------------------------
     * TODO, each printed so the grader can see it happened:
     *   - ia_pop_back on an empty array          (expect return 0)
     *   - ia_get / ia_set out of range           (expect ia_set -> 0)
     *   - ia_destroy called twice                (expect: no crash)
     *   - my_strcpy_safe into a too-small buffer (expect return 0, dst "")
     */

    
    printf("Failure path 1: ia_pop_back on an empty array\n");
    IntArray empty;
    ia_init(&empty);
    int popped = 0;
    printf("ia_pop_back(&empty, &popped) -> %d\n", ia_pop_back(&empty, &popped));
    ia_destroy(&empty);

    printf("Failure path 2: ia_set out of range\n");
    IntArray arr;
    ia_init(&arr);
    if (!ia_push_back(&arr, 42)) {
        printf("Failed to push initial value\n");
        ia_destroy(&arr);
        return 1;
    }
    printf("ia_set(&arr, 10, 99) -> %d\n", ia_set(&arr, 10, 99));
    ia_destroy(&arr);

    printf("Failure path 3: ia_destroy called twice\n");
    IntArray twice;
    ia_init(&twice);
    if (!ia_push_back(&twice, 7)) {
        printf("Failed to push test value\n");
        ia_destroy(&twice);
        return 1;
    }
    ia_destroy(&twice);
    ia_destroy(&twice);  /* should be harmless */
    printf("double ia_destroy completed without crashing\n");

    printf("Failure path 4: my_strcpy_safe into a too-small buffer\n");
    char dst[4] = {0};
    int ok = my_strcpy_safe(dst, sizeof(dst), "abcd");
    printf("my_strcpy_safe(dst, 4, \"abcd\") -> %d, dst=\"%s\"\n", ok, dst);
    return 0;
}
