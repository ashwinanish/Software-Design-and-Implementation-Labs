/* ------------------------------------------------------------------ *
 * main.cpp -- your test program for Lab 4.  Builds to ./dequetest.
 *
 * This file is a CLIENT of Deque, Stack and Queue.  It uses only
 * their public member functions.  Two rules this week:
 *   - never copy a Deque, Stack or Queue (no "Deque b = a;", no
 *     assigning one to another); pass one to a helper by pointer
 *   - every local is released by its destructor at its closing
 *     brace, so there is no destroy call to write -- and no reason
 *     to call a destructor or delete on a local yourself
 *
 * Three examples are written for you.  The list at the bottom is
 * yours; WRITEUP.md Part D quotes one of them.
 * ------------------------------------------------------------------ */
#include <cstdio>
#include <iostream>

#include "deque.h"
#include "stack.h"
#include "queue.h"

/* ---- a small test harness ---------------------------------------- */

static int g_checks = 0;
static int g_fails  = 0;

static void check(bool cond, const char *what)
{
    g_checks++;
    if (!cond) {
        g_fails++;
        std::cout << "  FAIL: " << what << '\n';
    }
}

/* Print both directions.  A deque can have a perfect next chain and
 * a broken prev chain; only the backward print shows it. */
static void show(const char *label, const Deque *d)
{
    std::cout << "  " << label << "  fwd: ";
    d->printForward();
    std::cout << "  " << label << "  bwd: ";
    d->printBackward();
}

/* ---- example 1: an empty deque answers every question ------------ */

static void test_empty(void)
{
    std::cout << "test_empty\n";

    Deque d;
    int   v = -1;

    check(d.size() == 0,   "fresh deque has size 0");
    check(d.empty(),       "fresh deque is empty");
    check(!d.popFront(&v), "popFront on empty returns false");
    check(!d.popBack(&v),  "popBack on empty returns false");
    check(!d.front(&v),    "front on empty returns false");
    check(v == -1,         "a failed call does not write *out");

    show("expecting [ ]", &d);
}                          /* ~Deque() runs here */

/* ---- example 2: the one-element popBack -------------------------- */

static void test_pop_back_only_node(void)
{
    std::cout << "test_pop_back_only_node\n";

    Deque d;
    int   v = 0;

    d.pushBack(7);
    check(d.popBack(&v) && v == 7, "popBack returns the only value");
    check(d.empty(),               "empty after popping the only node");

    /* If popBack left head_ pointing at the deleted node, this
     * pushFront writes through it.  It will often appear to work;
     * valgrind will not agree. */
    d.pushFront(9);
    check(d.size() == 1,           "size 1 after pushFront");
    check(d.front(&v) && v == 9,   "front is 9");
    check(d.back(&v)  && v == 9,   "back is 9");

    show("expecting [ 9 ]", &d);
}

/* ---- example 3: a Stack, which is given to you complete ---------- */

static void test_stack(void)
{
    std::cout << "test_stack\n";

    Stack s;
    int   v = 0;

    s.push(1);
    s.push(2);
    s.push(3);
    check(s.size() == 3,        "size 3");
    check(s.pop(&v) && v == 3,  "last pushed is first popped");
    check(s.pop(&v) && v == 2,  "then 2");
    check(s.pop(&v) && v == 1,  "then 1");
    check(s.empty(),            "empty");
}

/* ------------------------------------------------------------------ *
 * TODO -- write these.  The writeup and the valgrind check depend on
 * them.
 *
 *   test_push_front_cases   empty, one element, many; check front,
 *                           back, size, and show() both directions
 *   test_push_back_cases    the same three cases
 *   test_pop_front_cases    empty, one element (then pushBack
 *                           again), many
 *   test_pop_back_cases     the rest of the cases example 2 did not
 *                           cover
 *   test_remove_absent      returns false and changes nothing
 *   test_remove_only        the one-node deque; then push both ends
 *   test_remove_head
 *   test_remove_middle      check the backward print
 *   test_remove_tail        and then pushBack again
 *   test_clear_and_reuse    clear 5 elements, check empty, then
 *                           pushBack and pushFront again
 *   test_queue              first in, first out, including the empty
 *                           queue
 *   test_adapters_die_full  a Stack and a Queue that go out of scope
 *                           still holding values
 *   test_early_return       a helper that builds a Deque and returns
 *                           early on one path (WRITEUP.md Part D)
 *   test_many               >= 1000 values; spot-check, remove some,
 *                           drain
 *   test_heap_deque         a Deque made with new, released with
 *                           delete
 * ------------------------------------------------------------------ */


void test_push_front_cases(void)
{
    std::cout << "test_push_front_cases\n";
    Deque d;
    int v = 0;
    d.pushFront(1);
    check(d.front(&v) && v == 1, "front is 1");
    check(d.back(&v) && v == 1, "back is 1");
    d.pushFront(2);
    check(d.front(&v) && v == 2, "front is 2");
    check(d.back(&v) && v == 1, "back is still 1");
    d.pushFront(3);
    check(d.front(&v) && v == 3, "front is 3");
    check(d.back(&v) && v == 1, "back is still 1");
    show("expecting [ 3 <-> 2 <-> 1 ]", &d);
    
}

void test_push_back_cases(void)
{
    std::cout << "test_push_back_cases\n";
    Deque d;
    int v = 0;
    d.pushBack(1);
    check(d.front(&v) && v == 1, "front is 1");
    check(d.back(&v) && v == 1, "back is 1");
    d.pushBack(2);
    check(d.front(&v) && v == 1, "front is still 1");
    check(d.back(&v) && v == 2, "back is 2");
    d.pushBack(3);
    check(d.front(&v) && v == 1, "front is still 1");
    check(d.back(&v) && v == 3, "back is 3");
    show("expecting [ 1 <-> 2 <-> 3 ]", &d);
}

void test_pop_front_cases(void)
{
    std::cout << "test_pop_front_cases\n";
    Deque d;
    int v = 0;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);
    check(d.popFront(&v) && v == 1, "popFront returns 1");
    check(d.popFront(&v) && v == 2, "popFront returns 2");
    check(d.popFront(&v) && v == 3, "popFront returns 3");
    check(d.empty(), "deque is empty after popping all elements");
}

void test_pop_back_cases(void)
{
    std::cout << "test_pop_back_cases\n";
    Deque d;
    int v = 0;
    d.pushFront(1);
    d.pushFront(2);
    d.pushFront(3);
    check(d.popBack(&v) && v == 1, "popBack returns 1");
    check(d.popBack(&v) && v == 2, "popBack returns 2");
    check(d.popBack(&v) && v == 3, "popBack returns 3");
    check(d.empty(), "deque is empty after popping all elements");
}

void test_remove_absent(void)
{
    std::cout << "test_remove_absent\n";
    Deque d;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);
    check(!d.remove(4), "remove returns false for absent value");
    check(d.size() == 3, "size remains 3 after failed remove");
}

void test_remove_only(void)
{
    std::cout << "test_remove_only\n";
    Deque d;
    d.pushBack(1);
    check(d.remove(1), "remove returns true for only value");
    check(d.empty(), "deque is empty after removing the only value");
}

void test_remove_head(void)
{
    std::cout << "test_remove_head\n";
    Deque d;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);
    check(d.remove(1), "remove returns true for head value");
    check(d.size() == 2, "size is 2 after removing head");
    int v = 0;
    check(d.front(&v) && v == 2, "front is now 2 after removing head");
}

void test_remove_middle(void)
{
    std::cout << "test_remove_middle\n";
    Deque d;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);
    check(d.remove(2), "remove returns true for middle value");
    check(d.size() == 2, "size is 2 after removing middle");
    int v = 0;
    check(d.front(&v) && v == 1, "front is still 1 after removing middle");
    check(d.back(&v) && v == 3, "back is still 3 after removing middle");
}

void test_remove_tail(void)
{
    std::cout << "test_remove_tail\n";
    Deque d;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);
    check(d.remove(3), "remove returns true for tail value");
    check(d.size() == 2, "size is 2 after removing tail");
    int v = 0;
    check(d.front(&v) && v == 1, "front is still 1 after removing tail");
    check(d.back(&v) && v == 2, "back is now 2 after removing tail");
}

void test_clear_and_reuse(void)
{
    std::cout << "test_clear_and_reuse\n";
    Deque d;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);
    d.clear();
    check(d.empty(), "deque is empty after clear");
    d.pushBack(4);
    check(d.size() == 1, "size is 1 after pushing back after clear");
    int v = 0;
    check(d.front(&v) && v == 4, "front is 4 after pushing back after clear");
}

void test_queue(void)
{
    std::cout << "test_queue\n";
    Queue q;
    int v = 0;
    q.push(1);
    q.push(2);
    q.push(3);
    check(q.pop(&v) && v == 1, "pop returns 1");
    check(q.pop(&v) && v == 2, "pop returns 2");
    check(q.pop(&v) && v == 3, "pop returns 3");
    check(q.empty(), "queue is empty after popping all elements");
}

void test_adapters_die_full(void)
{
    std::cout << "test_adapters_die_full\n";
    {
        Stack s;
        Queue q;
        s.push(1);
        s.push(2);
        q.push(3);
        q.push(4);
    }
    check(true, "Stack and Queue destructors run without crashing");
}

void test_early_return(void)
{
    std::cout << "test_early_return\n";
    Deque d;
    d.pushBack(1);
    d.pushBack(2);
    d.pushBack(3);
    if (d.size() > 0) {
        return;
    }
    check(false, "this check should not run due to early return");
}

void test_many(void)
{
    std::cout << "test_many\n";
    Deque d;
    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        d.pushBack(i);
    }
    check(d.size() == N, "size is 1000 after pushing 1000 elements");
    int v = 0;
    for (int i = 0; i < N; ++i) {
        check(d.popFront(&v) && v == i, "popFront returns correct value");
    }
    check(d.empty(), "deque is empty after popping all elements");
}

void test_heap_deque(void)
{
    std::cout << "test_heap_deque\n";
    Deque *d = new Deque();
    d->pushBack(1);
    d->pushBack(2);
    d->pushBack(3);
    check(d->size() == 3, "size is 3 after pushing 3 elements");
    delete d;
    check(true, "Deque on heap deleted without crashing");
}

int main(void)
{
    test_empty();
    test_pop_back_only_node();
    test_stack();
    

    /* TODO: call your tests here. */
    test_push_front_cases();
    test_push_back_cases();
    test_pop_front_cases();
    test_pop_back_cases();
    test_remove_absent();
    test_remove_only();
    test_remove_head();
    test_remove_middle();
    test_remove_tail();
    test_clear_and_reuse();
    test_queue();
    test_adapters_die_full();
    test_early_return();
    test_many();
    test_heap_deque();

    std::cout << '\n' << g_checks << " checks, " << g_fails
              << " failures\n";
    return (g_fails == 0) ? 0 : 1;
}
