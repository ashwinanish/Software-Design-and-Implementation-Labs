/* ------------------------------------------------------------------ *
 * main.cpp -- your test program for Lab 3.  Builds to ./inventory.
 *
 * This file is a CLIENT of the list.  It may use only what list.h
 * declares.  It may not name the private struct, may not contain the
 * token "->next", and may not #include "list.cpp".  If you find
 * yourself wanting any of those, the list is missing an operation.
 *
 * Three examples are written for you.  The rest of the list at the
 * bottom is yours -- the rubric grades whether you covered them.
 * ------------------------------------------------------------------ */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "list.h"

/* ---- a two-line test harness, in the C subset ------------------- */

static int g_checks = 0;
static int g_fails  = 0;

static void check(int cond, const char *what)
{
    g_checks++;
    if (!cond) {
        g_fails++;
        printf("  FAIL: %s\n", what);
    }
}

/* ---- example 1: an empty list answers every question ------------ */

static void test_empty_list(void)
{
    printf("test_empty_list\n");

    List *l = List_create();
    check(l != NULL, "List_create returned NULL");
    if (l == NULL) return;

    check(List_size(l) == 0,        "fresh list size is 0");
    check(List_empty(l) == 1,       "fresh list is empty");
    check(List_find(l, 101) == NULL, "find on empty list is NULL");
    check(List_remove(l, 101) == 0, "remove on empty list returns 0");

    Item out;
    memset(&out, 0, sizeof out);
    check(List_remove_front(l, &out) == 0, "remove_front on empty is 0");

    printf("  expecting [ ] : ");
    List_print(l);

    List_destroy(l);
}

/* ---- example 2: the name is deep copied, not borrowed ----------- */

static void test_deep_copy(void)
{
    printf("test_deep_copy\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    char buf[16];
    strcpy(buf, "bolt");
    check(List_insert_front(l, 101, buf, 4) == 1, "insert_front ok");

    strcpy(buf, "SCRIBBLE");          /* clobber the caller's buffer */

    const Item *it = List_find(l, 101);
    check(it != NULL, "find finds the item we just inserted");
    if (it != NULL) {
        check(strcmp(it->name, "bolt") == 0,
              "stored name survives the caller clobbering its buffer");
        check(it->name != buf, "stored name is not the caller's pointer");
    }

    List_destroy(l);
}

/* ---- example 3: the stale-tail sequence ------------------------- */

static void test_stale_tail(void)
{
    printf("test_stale_tail\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 101, "bolt", 4) == 1, "insert_back ok");
    check(List_remove(l, 101) == 1,  "removed the only element");
    check(List_empty(l) == 1,        "list is empty again");

    /* If remove left tail pointing at the freed node, this write goes
     * through a dangling pointer.  It will often appear to work. */
    check(List_insert_back(l, 102, "nut", 9) == 1, "insert_back again");
    check(List_size(l) == 1,          "size is 1");

    const Item *it = List_find(l, 102);
    check(it != NULL && it->qty == 9, "the new item is really there");

    printf("  expecting [ (102,\"nut\",9) ] : ");
    List_print(l);

    List_destroy(l);
}

/* ------------------------------------------------------------------ *
 * TODO -- write these.  The rubric expects all of them.
 *
 *   test_single_element      remove it, then insert_back again, then
 *                            remove_front, then insert_front
 *   test_insert_interleaved  front/back/front/back, check the order
 *                            with List_print and with List_find
 *   test_remove_head
 *   test_remove_middle
 *   test_remove_tail         and then insert_back again
 *   test_remove_absent       returns 0 and changes nothing
 *   test_remove_front_owns   after remove_front, destroy the list,
 *                            then read out.name -- it must still be
 *                            valid -- then free it yourself
 *   test_clear_and_reuse     clear a 5-element list, check empty,
 *                            then insert_back and insert_front again
 *   test_many                build >= 1000 elements, check size and
 *                            a few spot finds, then destroy
 *   test_for_each_sum        sum the quantities through the ctx
 *                            pointer and check the total
 *
 * Every one of them must call List_destroy (or List_clear) on every
 * path out.  That is this week's whole point: you are the destructor.
 * ------------------------------------------------------------------ */

static void test_single_element(void)
{
    printf("test_single_element\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 1, "a", 1) == 1, "insert_back ok");
    check(List_remove(l, 1) == 1, "removed the only element");
    check(List_empty(l) == 1, "list is empty again");

    check(List_insert_back(l, 2, "b", 2) == 1, "insert_back again");
    check(List_size(l) == 1, "size is 1");
    check(List_find(l, 2) != NULL, "the new item is really there");

    Item out;
    memset(&out, 0, sizeof out);
    check(List_remove_front(l, &out) == 1, "remove_front ok");
    check(out.sku == 2 && out.qty == 2 && strcmp(out.name, "b") == 0,
          "got the right item from remove_front");
    free(out.name);

    check(List_insert_front(l, 3, "c", 3) == 1, "insert_front ok");
    check(List_size(l) == 1, "size is still 1");
    check(List_find(l, 3) != NULL, "the new item is really there");

    List_destroy(l);
}

static void test_insert_interleaved(void)
{
    printf("test_insert_interleaved\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_front(l, 1, "a", 1) == 1, "insert_front ok");
    check(List_insert_back(l, 2, "b", 2) == 1, "insert_back ok");
    check(List_insert_front(l, 3, "c", 3) == 1, "insert_front ok");
    check(List_insert_back(l, 4, "d", 4) == 1, "insert_back ok");

    printf("  expecting [ (3,\"c\",3) -> (1,\"a\",1) -> (2,\"b\",2) -> (4,\"d\",4) ] : ");
    List_print(l);

    check(List_find(l, 3) != NULL && List_find(l, 3)->qty == 3, "find c");
    check(List_find(l, 1) != NULL && List_find(l, 1)->qty == 1, "find a");
    check(List_find(l, 2) != NULL && List_find(l, 2)->qty == 2, "find b");
    check(List_find(l, 4) != NULL && List_find(l, 4)->qty == 4, "find d");

    List_destroy(l);
}

static void test_remove_head(void)
{
    printf("test_remove_head\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 1, "a", 1) == 1, "insert_back ok");
    check(List_insert_back(l, 2, "b", 2) == 1, "insert_back ok");
    check(List_insert_back(l, 3, "c", 3) == 1, "insert_back ok");

    check(List_remove(l, 1) == 1, "removed the head");
    check(List_size(l) == 2, "size is now 2");
    check(List_find(l, 1) == NULL, "head is gone");
    check(List_find(l, 2) != NULL && List_find(l, 2)->qty == 2, "middle intact");
    check(List_find(l, 3) != NULL && List_find(l, 3)->qty == 3, "tail intact");

    printf("  expecting [ (2,\"b\",2) -> (3,\"c\",3) ] : ");
    List_print(l);

    List_destroy(l);
}

static void test_remove_middle(void)
{
    printf("test_remove_middle\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 1, "a", 1) == 1, "insert_back ok");
    check(List_insert_back(l, 2, "b", 2) == 1, "insert_back ok");
    check(List_insert_back(l, 3, "c", 3) == 1, "insert_back ok");

    check(List_remove(l, 2) == 1, "removed the middle");
    check(List_size(l) == 2, "size is now 2");
    check(List_find(l, 2) == NULL, "middle is gone");
    check(List_find(l, 1) != NULL && List_find(l, 1)->qty == 1, "head intact");
    check(List_find(l, 3) != NULL && List_find(l, 3)->qty == 3, "tail intact");

    printf("  expecting [ (1,\"a\",1) -> (3,\"c\",3) ] : ");
    List_print(l);

    List_destroy(l);
}

static void test_remove_tail(void)
{
    printf("test_remove_tail\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 1, "a", 1) == 1, "insert_back ok");
    check(List_insert_back(l, 2, "b", 2) == 1, "insert_back ok");
    check(List_insert_back(l, 3, "c", 3) == 1, "insert_back ok");

    check(List_remove(l, 3) == 1, "removed the tail");
    check(List_size(l) == 2, "size is now 2");
    check(List_find(l, 3) == NULL, "tail is gone");
    check(List_find(l, 1) != NULL && List_find(l, 1)->qty == 1, "head intact");
    check(List_find(l, 2) != NULL && List_find(l, 2)->qty == 2, "middle intact");

    printf("  expecting [ (1,\"a\",1) -> (2,\"b\",2) ] : ");
    List_print(l);

    List_destroy(l);
}

static void test_remove_absent(void)
{
    printf("test_remove_absent\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 1, "a", 1) == 1, "insert_back ok");
    check(List_insert_back(l, 2, "b", 2) == 1, "insert_back ok");

    check(List_remove(l, 99) == 0, "remove absent returns 0");
    check(List_size(l) == 2, "size unchanged");
    check(List_find(l, 1) != NULL && List_find(l, 1)->qty == 1, "head intact");
    check(List_find(l, 2) != NULL && List_find(l, 2)->qty == 2, "tail intact");

    printf("  expecting [ (1,\"a\",1) -> (2,\"b\",2) ] : ");
    List_print(l);

    List_destroy(l);
}

static void test_remove_front_owns(void)
{
    printf("test_remove_front_owns\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 1, "a", 1) == 1, "insert_back ok");
    check(List_insert_back(l, 2, "b", 2) == 1, "insert_back ok");

    Item out;
    memset(&out, 0, sizeof out);
    check(List_remove_front(l, &out) == 1, "remove_front ok");
    check(out.sku == 1 && out.qty == 1 && strcmp(out.name, "a") == 0,
          "got the right item from remove_front");

    List_destroy(l);
    check(strcmp(out.name, "a") == 0, "the removed item's name is still valid after destroy");
}

static void test_clear_and_reuse(void)
{
    printf("test_clear_and_reuse\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 0, "item0", 0) == 1, "insert_back ok");
    check(List_insert_back(l, 1, "item1", 1) == 1, "insert_back ok");
    check(List_insert_back(l, 2, "item2", 2) == 1, "insert_back ok");
    check(List_insert_back(l, 3, "item3", 3) == 1, "insert_back ok");
    check(List_insert_back(l, 4, "item4", 4) == 1, "insert_back ok");

    List_clear(l);
    check(List_size(l) == 0, "size is 0 after clear");
    check(List_empty(l) == 1, "list is empty after clear");

    check(List_insert_back(l, 10, "b", 10) == 1, "insert_back after clear ok");
    check(List_insert_front(l, 9, "a", 9) == 1, "insert_front after clear ok");

    printf("  expecting [ (9,\"a\",9) -> (10,\"b\",10) ] : ");
    List_print(l);

    List_destroy(l);
}

static void test_many(void)
{
    printf("test_many\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    const int N = 1000;
    for (int i = 0; i < N; i++) {
        check(List_insert_back(l, i, "item", i) == 1, "insert_back ok");
    }

    check(List_size(l) == N, "size is N after many inserts");

    for (int i = 0; i < N; i += 100) {
        const Item *it = List_find(l, i);
        check(it != NULL && it->qty == i, "find works for some items");
    }

    List_destroy(l);
}

static void test_for_each_sum(void)
{
    printf("test_for_each_sum\n");

    List *l = List_create();
    if (l == NULL) { check(0, "List_create returned NULL"); return; }

    check(List_insert_back(l, 1, "item1", 1) == 1, "insert_back ok");
    check(List_insert_back(l, 2, "item2", 2) == 1, "insert_back ok");
    check(List_insert_back(l, 3, "item3", 3) == 1, "insert_back ok");
    check(List_insert_back(l, 4, "item4", 4) == 1, "insert_back ok");
    check(List_insert_back(l, 5, "item5", 5) == 1, "insert_back ok");

    long total = 0;
    List_for_each(l, [](const Item *it, void *ctx) {
        long *sum = (long *)ctx;
        *sum += it->qty;
    }, &total);

    check(total == 15, "total quantity is correct (1+2+3+4+5=15)");

    List_destroy(l);
}

int main(void)
{
    test_empty_list();
    test_deep_copy();
    test_stale_tail();

    test_single_element();
    test_insert_interleaved();
    test_remove_head();
    test_remove_middle();
    test_remove_tail();
    test_remove_absent();
    test_remove_front_owns();
    test_clear_and_reuse();
    test_many();
    test_for_each_sum();

    printf("\n%d checks, %d failures\n", g_checks, g_fails);
    return (g_fails == 0) ? 0 : 1;
}

