/* ------------------------------------------------------------------ *
 * stack.h -- ECE 412 Lab 4.  A stack built by composition: it
 *            contains a Deque and lets you touch only one end of it.
 *
 * DO NOT MODIFY THIS FILE.  It is complete.  Read it, then write
 * queue.h the same way.
 * ------------------------------------------------------------------ */
#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include "deque.h"

class Stack {
public:
    void push(int v)          { d_.pushFront(v); }
    bool pop (int *out)       { return d_.popFront(out); }
    bool top (int *out) const { return d_.front(out); }
    std::size_t size()  const { return d_.size(); }
    bool        empty() const { return d_.empty(); }
private:
    Deque d_;              // a Stack contains a Deque
};

#endif /* STACK_H */
