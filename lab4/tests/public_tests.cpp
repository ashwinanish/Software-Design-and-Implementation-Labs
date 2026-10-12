/* ------------------------------------------------------------------ *
 * tests/public_tests.cpp -- ECE 412 Lab 4 provided test suite.
 *
 *   build:  g++ -std=c++11 -Wall -Wextra -Werror -g -I. \
 *               tests/public_tests.cpp deque.o -o public_tests
 *   run:    ./public_tests
 *   memory: valgrind --leak-check=full ./public_tests
 *
 * (Once your Makefile has "check" and "vcheck" targets, those two
 * commands become "make check" and "make vcheck".)
 *
 * These are the same KINDS of tests the autograder runs.  They are
 * not the whole autograder: passing all of them is necessary, not
 * sufficient.  In particular they do not time size() or pushBack(),
 * they do not check your Makefile or your writeup, and the
 * autograder runs several sequences that are deliberately not in
 * here.
 *
 * You do not submit this file, and you should not edit it.  Your own
 * tests belong in main.cpp.
 * ------------------------------------------------------------------ */
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>     /* dup, dup2 -- used only to capture stdout */

#include "deque.h"
#include "stack.h"
#include "queue.h"

/* ---- tiny harness ----------------------------------------------- */

static int g_checks = 0;
static int g_fails  = 0;
static const char *g_test = "";

static void ck(bool cond, const char *what)
{
    g_checks++;
    if (!cond) {
        g_fails++;
        printf("  FAIL  [%s] %s\n", g_test, what);
    }
}

static void begin(const char *name)
{
    g_test = name;
    printf("%s\n", name);
}

/* ---- capture what printForward / printBackward write ------------- */

/* Collapse runs of whitespace to one space and trim both ends, so a
 * stray extra space does not fail you.  The shape still must match. */
static void squeeze(char *s)
{
    int i = 0, j = 0, sp = 0;
    while (s[i] != '\0') {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' ||
            s[i] == '\r') {
            sp = 1;
        } else {
            if (sp && j > 0) s[j++] = ' ';
            sp = 0;
            s[j++] = s[i];
        }
        i++;
    }
    s[j] = '\0';
}

/* Run d->printForward() (or printBackward) with stdout sent to buf. */
static void capture(const Deque *d, bool forward, char *buf, size_t cap)
{
    const char *path = "._print_capture.tmp";
    int   saved;
    FILE *f;
    long  n;

    buf[0] = '\0';
    f = fopen(path, "w+");
    if (f == NULL) return;

    std::cout.flush();
    fflush(stdout);
    saved = dup(1);
    if (saved < 0) { fclose(f); return; }
    if (dup2(fileno(f), 1) < 0) { close(saved); fclose(f); return; }

    if (forward) d->printForward();
    else         d->printBackward();

    std::cout.flush();
    fflush(stdout);
    dup2(saved, 1);
    close(saved);

    fseek(f, 0, SEEK_END);
    n = ftell(f);
    if (n < 0) n = 0;
    if ((size_t)n > cap - 1) n = (long)cap - 1;
    fseek(f, 0, SEEK_SET);
    if (fread(buf, 1, (size_t)n, f) != (size_t)n) { /* short read */ }
    buf[n] = '\0';
    fclose(f);
    remove(path);
    squeeze(buf);
}

static void ck_print(const Deque *d, bool forward, const char *expect)
{
    char got[4096];
    capture(d, forward, got, sizeof got);
    g_checks++;
    if (strcmp(got, expect) != 0) {
        g_fails++;
        printf("  FAIL  [%s] %s\n", g_test,
               forward ? "printForward" : "printBackward");
        printf("          expected: %s\n", expect);
        printf("          got:      %s\n", got);
    }
}

/* Check both directions at once: the prev chain is checked by the
 * backward print, and a list can have a perfect next chain and a
 * broken prev chain. */
static void ck_both(const Deque *d, const char *fwd, const char *bwd)
{
    ck_print(d, true,  fwd);
    ck_print(d, false, bwd);
}

/* ================================================================== *
 * Deque: the ends
 * ================================================================== */

static void t_empty(void)
{
    begin("t_empty -- every operation on a fresh deque");

    Deque d;
    int   v = 12345;

    ck(d.size() == 0,       "size 0");
    ck(d.empty(),           "empty");
    ck(!d.popFront(&v),     "popFront -> false");
    ck(!d.popBack(&v),      "popBack -> false");
    ck(!d.front(&v),        "front -> false");
    ck(!d.back(&v),         "back -> false");
    ck(v == 12345,          "a failed pop/peek must not write *out");
    ck(!d.contains(0),      "contains -> false");
    ck(!d.remove(0),        "remove -> false");
    ck_both(&d, "[ ]", "[ ]");

    d.clear();                             /* clear of empty is fine */
    ck(d.size() == 0,       "size still 0 after clear");
}

static void t_push_front_order(void)
{
    begin("t_push_front_order");

    Deque d;
    int   v = 0;
    d.pushFront(1);
    d.pushFront(2);
    d.pushFront(3);

    ck(d.size() == 3,           "size 3");
    ck(!d.empty(),              "not empty");
    ck(d.front(&v) && v == 3,   "front is 3");
    ck(d.back(&v)  && v == 1,   "back is 1");
    ck_both(&d, "[ 3 <-> 2 <-> 1 ]", "[ 1 <-> 2 <-> 3 ]");
}

static void t_push_back_order(void)
{
    begin("t_push_back_order");

    Deque d;
    int   v = 0;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);

    ck(d.size() == 3,           "size 3");
    ck(d.front(&v) && v == 1,   "front is 1");
    ck(d.back(&v)  && v == 3,   "back is 3");
    ck_both(&d, "[ 1 <-> 2 <-> 3 ]", "[ 3 <-> 2 <-> 1 ]");
}

/* pushFront on an EMPTY deque must also set tail_. */
static void t_front_then_back(void)
{
    begin("t_front_then_back -- tail_ after a pushFront on empty");

    Deque d;
    d.pushFront(1);
    d.pushBack(2);
    ck(d.size() == 2, "size 2");
    ck_both(&d, "[ 1 <-> 2 ]", "[ 2 <-> 1 ]");
}

/* pushBack on an EMPTY deque must also set head_. */
static void t_back_then_front(void)
{
    begin("t_back_then_front -- head_ after a pushBack on empty");

    Deque d;
    d.pushBack(1);
    d.pushFront(0);
    ck(d.size() == 2, "size 2");
    ck_both(&d, "[ 0 <-> 1 ]", "[ 1 <-> 0 ]");
}

static void t_interleaved(void)
{
    begin("t_interleaved -- front/back/front/back");

    Deque d;
    d.pushBack(2);
    d.pushFront(1);
    d.pushBack(3);
    d.pushFront(0);
    ck(d.size() == 4, "size 4");
    ck_both(&d, "[ 0 <-> 1 <-> 2 <-> 3 ]", "[ 3 <-> 2 <-> 1 <-> 0 ]");
}

/* The trap from the handout: popBack on ONE node must null head_ too. */
static void t_pop_back_one_then_front(void)
{
    begin("t_pop_back_one_then_front -- the one-element popBack");

    Deque d;
    int   v = 0;
    d.pushBack(7);
    ck(d.popBack(&v) && v == 7, "popBack returns 7");
    ck(d.empty(),               "empty after popping the only node");
    ck_both(&d, "[ ]", "[ ]");

    d.pushFront(9);             /* writes through head_ if it dangles */
    ck(d.size() == 1,           "size 1");
    ck_both(&d, "[ 9 ]", "[ 9 ]");
}

static void t_pop_front_one_then_back(void)
{
    begin("t_pop_front_one_then_back -- the one-element popFront");

    Deque d;
    int   v = 0;
    d.pushFront(7);
    ck(d.popFront(&v) && v == 7, "popFront returns 7");
    ck(d.empty(),                "empty after popping the only node");

    d.pushBack(9);              /* writes through tail_ if it dangles */
    ck(d.size() == 1,            "size 1");
    ck_both(&d, "[ 9 ]", "[ 9 ]");
}

static void t_pop_both_ends(void)
{
    begin("t_pop_both_ends");

    Deque d;
    int   v = 0;
    for (int i = 1; i <= 5; ++i) d.pushBack(i);    /* 1 2 3 4 5 */

    ck(d.popFront(&v) && v == 1, "popFront 1");
    ck(d.popBack(&v)  && v == 5, "popBack 5");
    ck(d.size() == 3,            "size 3");
    ck_both(&d, "[ 2 <-> 3 <-> 4 ]", "[ 4 <-> 3 <-> 2 ]");

    ck(d.popBack(&v)  && v == 4, "popBack 4");
    ck(d.popFront(&v) && v == 2, "popFront 2");
    ck(d.popBack(&v)  && v == 3, "popBack 3 (the last one)");
    ck(!d.popFront(&v),          "now empty");
    ck(d.size() == 0,            "size 0");
    ck_both(&d, "[ ]", "[ ]");
}

static void t_peek_does_not_remove(void)
{
    begin("t_peek_does_not_remove");

    Deque d;
    int   v = 0;
    d.pushBack(4);
    d.pushBack(8);
    ck(d.front(&v) && v == 4, "front 4");
    ck(d.front(&v) && v == 4, "front 4 again");
    ck(d.back(&v)  && v == 8, "back 8");
    ck(d.size() == 2,         "still size 2");
}

static void t_clear_and_reuse(void)
{
    begin("t_clear_and_reuse");

    Deque d;
    for (int i = 0; i < 5; ++i) d.pushBack(i);
    d.clear();
    ck(d.empty() && d.size() == 0, "empty after clear");
    ck_both(&d, "[ ]", "[ ]");

    d.pushBack(1);              /* a stale tail_ breaks this */
    d.pushFront(0);             /* a stale head_ breaks this */
    ck(d.size() == 2,            "size 2 after reuse");
    ck_both(&d, "[ 0 <-> 1 ]", "[ 1 <-> 0 ]");
}

/* ================================================================== *
 * Deque: contains and remove
 * ================================================================== */

static void fill123(Deque *d)
{
    d->pushBack(1);
    d->pushBack(2);
    d->pushBack(3);
}

static void t_contains(void)
{
    begin("t_contains");

    Deque d;
    fill123(&d);
    ck(d.contains(1),  "contains head");
    ck(d.contains(2),  "contains middle");
    ck(d.contains(3),  "contains tail");
    ck(!d.contains(4), "does not contain 4");
    ck(d.size() == 3,  "contains does not change size");
}

static void t_remove_head(void)
{
    begin("t_remove_head");

    Deque d;
    fill123(&d);
    ck(d.remove(1),     "removed the head");
    ck(d.size() == 2,   "size 2");
    ck(!d.contains(1),  "1 is gone");
    ck_both(&d, "[ 2 <-> 3 ]", "[ 3 <-> 2 ]");

    d.pushFront(0);
    ck_both(&d, "[ 0 <-> 2 <-> 3 ]", "[ 3 <-> 2 <-> 0 ]");
}

static void t_remove_middle(void)
{
    begin("t_remove_middle");

    Deque d;
    fill123(&d);
    ck(d.remove(2),     "removed the middle");
    ck(d.size() == 2,   "size 2");
    ck_both(&d, "[ 1 <-> 3 ]", "[ 3 <-> 1 ]");
}

static void t_remove_tail(void)
{
    begin("t_remove_tail -- and then pushBack again");

    Deque d;
    fill123(&d);
    ck(d.remove(3),     "removed the tail");
    ck(d.size() == 2,   "size 2");
    ck_both(&d, "[ 1 <-> 2 ]", "[ 2 <-> 1 ]");

    d.pushBack(4);              /* writes through tail_ if it dangles */
    ck_both(&d, "[ 1 <-> 2 <-> 4 ]", "[ 4 <-> 2 <-> 1 ]");
}

static void t_remove_only(void)
{
    begin("t_remove_only -- the one-element deque");

    Deque d;
    d.pushBack(7);
    ck(d.remove(7),     "removed the only node");
    ck(d.empty(),       "empty");
    ck_both(&d, "[ ]", "[ ]");

    d.pushFront(1);
    d.pushBack(2);
    ck_both(&d, "[ 1 <-> 2 ]", "[ 2 <-> 1 ]");
}

static void t_remove_absent(void)
{
    begin("t_remove_absent -- returns false and changes nothing");

    Deque d;
    fill123(&d);
    ck(!d.remove(99),   "remove 99 -> false");
    ck(d.size() == 3,   "size unchanged");
    ck_both(&d, "[ 1 <-> 2 <-> 3 ]", "[ 3 <-> 2 <-> 1 ]");
}

static void t_remove_first_only(void)
{
    begin("t_remove_first_only -- duplicates");

    Deque d;
    d.pushBack(5);
    d.pushBack(1);
    d.pushBack(5);
    d.pushBack(2);
    ck(d.remove(5),     "removed one 5");
    ck(d.size() == 3,   "size 3");
    ck(d.contains(5),   "the second 5 is still there");
    ck_both(&d, "[ 1 <-> 5 <-> 2 ]", "[ 2 <-> 5 <-> 1 ]");
}

/* ================================================================== *
 * Stack and Queue
 * ================================================================== */

static void t_stack_lifo(void)
{
    begin("t_stack_lifo");

    Stack s;
    int   v = 0;
    ck(s.empty() && !s.pop(&v) && !s.top(&v), "fresh stack is empty");

    s.push(1);
    s.push(2);
    s.push(3);
    ck(s.size() == 3,          "size 3");
    ck(s.top(&v) && v == 3,    "top 3");
    ck(s.pop(&v) && v == 3,    "pop 3");
    ck(s.pop(&v) && v == 2,    "pop 2");
    s.push(4);
    ck(s.pop(&v) && v == 4,    "pop 4");
    ck(s.pop(&v) && v == 1,    "pop 1");
    ck(s.empty(),              "empty again");
}

static void t_queue_fifo(void)
{
    begin("t_queue_fifo");

    Queue q;
    int   v = 0;
    ck(q.empty() && q.size() == 0, "fresh queue is empty");
    ck(!q.pop(&v),                 "pop on empty -> false");
    ck(!q.front(&v),               "front on empty -> false");

    q.push(1);
    q.push(2);
    q.push(3);
    ck(q.size() == 3,           "size 3");
    ck(q.front(&v) && v == 1,   "front 1");
    ck(q.pop(&v) && v == 1,     "pop 1");
    ck(q.pop(&v) && v == 2,     "pop 2");
    q.push(4);
    ck(q.pop(&v) && v == 3,     "pop 3");
    ck(q.pop(&v) && v == 4,     "pop 4");
    ck(q.empty(),               "empty again");
    ck(!q.pop(&v),              "pop on drained queue -> false");
}

/* ================================================================== *
 * bigger
 * ================================================================== */

static void t_many(void)
{
    begin("t_many -- 1000 nodes, remove every third");

    Deque d;
    int   v = 0;
    for (int i = 0; i < 1000; ++i) d.pushBack(i);
    ck(d.size() == 1000,          "size 1000");
    ck(d.front(&v) && v == 0,     "front 0");
    ck(d.back(&v)  && v == 999,   "back 999");

    for (int i = 0; i < 1000; i += 3) ck(d.remove(i), "remove 3k");
    ck(d.size() == 1000 - 334,    "size after removing every third");
    ck(!d.contains(999),          "999 was a multiple of 3");
    ck(d.back(&v) && v == 998,    "998 is the new back");

    /* drain from the back and check the order */
    int  expect = 998;
    bool ok = true;
    while (d.popBack(&v)) {
        if (v != expect) ok = false;
        expect -= (expect % 3 == 1) ? 2 : 1;
    }
    ck(ok,          "popBack drains in reverse order, skipping 3k");
    ck(d.empty(),   "empty after the drain");
}

/* A Deque created with new is destroyed only by delete. */
static void t_heap_deque(void)
{
    begin("t_heap_deque -- new and delete a Deque");

    Deque *d = new Deque;
    for (int i = 0; i < 10; ++i) d->pushFront(i);
    ck(d->size() == 10, "size 10");
    delete d;           /* runs ~Deque(), which must delete 10 nodes */
    ck(true, "delete returned");
}

/* ================================================================== */

int main(void)
{
    t_empty();
    t_push_front_order();
    t_push_back_order();
    t_front_then_back();
    t_back_then_front();
    t_interleaved();
    t_pop_back_one_then_front();
    t_pop_front_one_then_back();
    t_pop_both_ends();
    t_peek_does_not_remove();
    t_clear_and_reuse();

    t_contains();
    t_remove_head();
    t_remove_middle();
    t_remove_tail();
    t_remove_only();
    t_remove_absent();
    t_remove_first_only();

    t_stack_lifo();
    t_queue_fifo();

    t_many();
    t_heap_deque();

    printf("\n----------------------------------------\n");
    printf("%d checks, %d failures\n", g_checks, g_fails);
    if (g_fails == 0)
        printf("all provided tests passed "
               "(now run it under valgrind)\n");
    return (g_fails == 0) ? 0 : 1;
}
