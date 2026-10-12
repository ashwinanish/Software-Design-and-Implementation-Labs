/* ------------------------------------------------------------------ *
 * queue.h -- ECE 412 Lab 4.  A queue built by composition.
 *
 * First in, first out: push at the back, pop from the front.  Write
 * it the way stack.h is written -- read that file first.
 *
 * Rules (the autograder checks every one):
 *   - Queue contains a Deque BY VALUE, as its only, private, data
 *     member.
 *   - No Node, no new/delete, no "->", no pointer other than passing
 *     `out` straight through to the Deque.
 *   - No constructor and no destructor.  WRITEUP.md asks you why
 *     none is needed.
 *
 * The declarations below are the required interface; keep them.
 * ------------------------------------------------------------------ */
#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include "deque.h"

class Queue {
public:
    /* TODO: each body is one line that forwards to the Deque. */
    void push (int v)          { d_.pushBack(v); }
    bool pop  (int *out)       { return d_.popFront(out); }
    bool front(int *out) const { return d_.front(out); }
    std::size_t size()  const  { return d_.size(); }
    bool empty()        const  { return d_.empty(); }
private:
    Deque d_;
};

#endif /* QUEUE_H */
