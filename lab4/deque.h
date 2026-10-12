/* ------------------------------------------------------------------ *
 * deque.h -- ECE 412 Lab 4: a doubly linked list of int, used as a
 *            double-ended queue.
 *
 * DO NOT MODIFY THIS FILE.  The autograder compiles its own test
 * programs against these exact declarations, including the private
 * section: the size and layout of a Deque come from this header, so
 * a deque.cpp written against a different one will not link.
 * ------------------------------------------------------------------ */
#ifndef DEQUE_H
#define DEQUE_H

#include <cstddef>

class Deque {
public:
    Deque();                              // empty deque
    ~Deque();                             // deletes every node

    void   pushFront(int v);
    void   pushBack (int v);
    bool   popFront(int *out);            // false if empty
    bool   popBack (int *out);            // false if empty

    bool   front(int *out) const;         // peek, no removal
    bool   back (int *out) const;

    bool   contains(int v) const;         // true if some node holds v
    bool   remove(int v);                 // unlink and delete the FIRST
                                          // node holding v; false if none

    std::size_t size()  const;            // keep a counter; never walk
    bool        empty() const;
    void        clear();                  // delete every node; the deque
                                          // stays usable afterwards

    void printForward()  const;           // [ 1 <-> 2 <-> 3 ]
    void printBackward() const;           // [ 3 <-> 2 <-> 1 ]

private:
    struct Node {
        int   value;
        Node *prev;
        Node *next;
        explicit Node(int v) : value(v), prev(NULL), next(NULL) {}
    };
    Node       *head_;
    Node       *tail_;
    std::size_t size_;
};

#endif /* DEQUE_H */
