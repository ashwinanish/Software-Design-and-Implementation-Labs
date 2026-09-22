/*
 * ================================================================
 * Filename:    main.cpp
 * Description: ECE 412 Lab 2 -- driver. Exercises the ADT through
 *              the six numbered activities from the handout. This
 *              whole program must be valgrind-clean, and must NEVER
 *              touch db->anything -- if you cannot spell out the
 *              struct's fields, that is the ADT working correctly.
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

#include "db.h"
#include "mystr.h"

int main(void) {
    /* ---- 1. Create + grow ----------------------------------------------
     * TODO: DB_create(4). Add >= 20 records (id, "name<i>", gpa) so the
     * array doubles at least twice (4 -> 8 -> 16 -> 32). After each add,
     * or every few adds, print DB_size and DB_capacity so the two
     * doublings are visible in the output.
     */
    DB *db = DB_create(4);
    for (int i = 0; i < 20; i++) {
        char name[6];
        for(int j = 0; j < 5; j++) {
            name[j] = 'a' + i;
        }
        name[5] = '\0';
        double gpa = 4.0 - (i * 0.1);
        if (!DB_add(db, i, name, gpa)) {
            printf("Failed to add record %d\n", i);
        }
        printf("After adding record %d: size=%zu capacity=%zu\n", i, DB_size(db), DB_capacity(db));
    }
    /* ---- 2. Find present + absent --------------------------------------
     * TODO: DB_find an id you added (print id/name/gpa from the returned
     * const Record *). DB_find an id you did NOT add (print "not found").
     * Note that you never write through the pointer -- it is const on
     * purpose; explain why in your README.
     */
    const Record *r = DB_find(db, 5);
    if (r) {
        printf("Found record: id=%d name=%s gpa=%.2f\n", r->id, r->name, r->gpa);
    } 
    else {
        printf("Record with id=5 not found\n");
    }
    r = DB_find(db, 100);
    if (r) {
        printf("Found record: id=%d name=%s gpa=%.2f\n", r->id, r->name, r->gpa);
    } 
    else {
        printf("Record with id=100 not found\n");
    }
    /* ---- 3. Remove first, middle, last ---------------------------------
     * TODO: DB_remove the first id, DB_remove a middle id, DB_remove the
     * last id. After each removal call DB_print to show the DB. Removing
     * from the middle without leaving a hole is the interesting case --
     * your README states whether you shift the tail down or swap the
     * last element into the gap.
     */
    DB_remove(db, 0);
    printf("After removing first record:\n");
    DB_print(db);
    DB_remove(db, 10);
    printf("After removing middle record:\n");
    DB_print(db);
    DB_remove(db, 19);
    printf("After removing last record:\n");
    DB_print(db);
    /* ---- 4. Duplicate add fails ----------------------------------------
     * TODO: DB_add an id that is still present. Check that the return
     * value is 0 and DB_size did not change.
     */
    int result = DB_add(db, 5, "duplicate", 3.0);
    if (result == 0) {
        printf("Duplicate add failed as expected. Size=%zu\n", DB_size(db));
    } 
    else {
        printf("Duplicate add succeeded unexpectedly. Size=%zu\n", DB_size(db));
    }
    /* ---- 5. Save + destroy + create + load round trip ------------------
     * TODO:
     *   - DB_save(db, "records.txt")
     *   - DB_destroy(db); db = NULL;   // hygiene
     *   - db = DB_create(4)
     *   - DB_load(db, "records.txt")
     *   - DB_print(db) -- must match what you saved
     */
    if (DB_save(db, "records.txt")) {
        printf("Database saved successfully.\n");
    } 
    else {
        printf("Failed to save database.\n");
    }
    DB_destroy(db);
    db = DB_create(4);
    if (DB_load(db, "records.txt")) {
        printf("Database loaded successfully.\n");
    } 
    else {
        printf("Failed to load database.\n");
    }
    printf("Database after loading:\n");
    DB_print(db);
    DB_destroy(db);
    db = NULL;
    /* ---- 6. Malformed file fails cleanly -------------------------------
     * TODO: DB_load(db, "bad_records.txt"). Check the return value is 0.
     * The DB must still be usable after this call (no crash, no leak).
     * Then DB_destroy the final DB so the whole program is
     * valgrind-clean.
     */

    db = DB_create(4);
    if (DB_load(db, "bad_records.txt") == 0) {
        printf("Malformed file load failed as expected.\n");
    }
    else {
        printf("Malformed file load succeeded unexpectedly.\n");
    }
    DB_destroy(db);
    return 0;
}
