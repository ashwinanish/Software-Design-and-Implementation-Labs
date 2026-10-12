# ECE 412 -- Lab 4: Doubly Linked List, Deque, Stack & Queue

| | |
|---|---|
| **Assigned** | Mon Sep 28 |
| **Due** | **Mon Oct 5, 11:59 pm** (Gradescope) |
| **Weight** | 1% |
| **Builds on** | L8 (linked structures, doubly linked lists, costs from the diagram) · L9 (classes, `private`/`public`, `const` members) · L10 (constructors, destructors, `new`/`delete`, RAII) |
| **Language** | C++11 — `g++ -std=c++11 -Wall -Wextra -Werror -g` |
| **Work** | Individual |

> **Last lab before Midterm 1** (Thu Oct 8). Everything here — the pointer surgery, the destructor, the cost table — is directly examinable on paper.

## Goal

In Lab 3 you built a singly linked list in which you could only ever move forward and removing a node meant walking from the head to find its predecessor. This week you add the `prev` pointer and watch three things happen at once:

1. **Unlinking a node stops needing its predecessor search.** In a singly linked list you must walk from `head` to find the node *before* the one you are removing. In a doubly linked list the node already holds that pointer, and the unlink itself is a handful of pointer writes, the same handful for any list length.
2. **The back of the list becomes as cheap as the front.** With a tail pointer *and* `prev`, all four end operations take a fixed number of steps, no matter how long the list is. That is a **deque** (double-ended queue).
3. **Stack and queue stop being data structures and become interfaces.** Once you have a deque, a stack is a deque that only lets you touch one end, and a queue is a deque that pushes at one end and pops at the other. You do not need to write a single new pointer manipulation to build them.

That third point is the real lesson of this lab: **most "new" data structures are old data structures with a restricted interface.**

There is a fourth change, and it is the one Lab 3's README asked you to predict: this list is a **class with a destructor**. In Lab 3 you called `List_destroy` on every path out of every test. This week there is no such call to forget.

## What is in the repository

| File | What you do with it |
|---|---|
| `deque.h` | **Do not modify.** The `Deque` class, private section included. |
| `deque.cpp` | Your implementation. Every member function is stubbed with a TODO. |
| `stack.h` | **Do not modify.** Complete: read it before you write `queue.h`. |
| `queue.h` | The required declarations, with empty bodies and no data member. You finish it. |
| `main.cpp` | Your tests. Three are written for you; the list of the rest is at the bottom. |
| `tests/public_tests.cpp` | A provided suite. Do not edit it, do not submit it. |
| `DIAGRAMS.md`, `WRITEUP.md`, `AI_LOG.md` | Templates for the three reports. Fill them in. |

There is no `Makefile`. Lab 3's was written for you; this one you write (see **The Makefile**, below).

The starter code compiles clean with `-Wall -Wextra -Werror` as shipped and fails its tests at run time.

## Part A — `Deque` (the doubly-linked list)

A class this time, not an opaque struct. It owns its nodes, its constructor sets it up, and its destructor releases them.

```cpp
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

#endif
```

This header is `deque.h` in the repository. Use it exactly as given; the autograder compiles its own tests against it. Notes on what is in it:

- **`Node` is declared inside the `private:` section.** Lab 3 hid `struct Node` by keeping it out of the header. Here the class wall does the same job: only `Deque`'s member functions can name `Deque::Node`, and a client that tries gets a compile error.
- **`Node` has its own constructor**, so `new Node(v)` produces a node whose `prev` and `next` are already `NULL`. 
- **`pop`, `front`, and `back` report through a pointer.** On success they write the value to `*out` and return `true`; on an empty deque they return `false` and do not touch `*out`. `out` must point at an `int`.
- **`printForward` / `printBackward`** print with `std::cout` on one line followed by `'\n'`: `"[ "`, the values separated by `" <-> "`, then `" ]"`. An empty deque prints `[ ]`. Whitespace runs are compared loosely; the punctuation must match.

### Constructor and destructor

- The constructor sets `head_`, `tail_`, and `size_` in a **member-initializer list**, written in declaration order (`-Werror=reorder` rejects any other order).
- The destructor must `delete` every node. What function already does that and can you just invoke that in the destructor?
- The freeing loop is Lab 3's, with `delete` in place of `free`.

> **Do not copy a `Deque` (or a `Stack` or a `Queue`) this week.**
>
> ```cpp
> Deque a;
> a.pushBack(1);
> Deque b = a;        // compiles.  Do not write this.
> ```
>
> The compiler writes a copy for you, and it copies the *pointers* — so `a` and `b` share the same nodes, and both destructors delete them. Pass a deque to a helper function by pointer only (`void fill(Deque *d)`), and never assign one to another. The autograder does not copy.

### `remove(v)` — the doubly-linked unlink

Walk from `head_` to the first node holding `v` (there is no way to skip ahead), then unlink it **without a `prev` variable** — the node already knows both neighbours. Four cases, and you must test all four:

1. the value is **not present**: return `false`, change nothing;
2. the node is the **only** node: `head_` and `tail_` both become `NULL`;
3. the node is the **head** (or the **tail**) of a longer list: `head_` (or `tail_`) moves, and the new end node's `prev` (or `next`) becomes `NULL`;
4. the node is in the **middle**: its two neighbours are pointed at each other.

The symmetric form from lecture covers all four with two `if`/`else` pairs. 

### Required: draw before you code

For each of `pushFront`, `pushBack`, `popFront`, `popBack`, draw the box-and-arrow diagram for **three cases**: empty list, one-element list, many-element list. Also draw `remove` for a **head**, a **middle**, and a **tail** node. Put these diagrams in `DIAGRAMS.md` (ASCII is fine, a photo of neat paper is fine).

This is not busywork and it is not optional — **it is 10% of the lab grade**. The one-element case is where almost every doubly-linked bug lives, because that is the case where `head_` and `tail_` are the same node and updating one without the other leaves you with a corrupt list that *still passes small tests*.

```text
pushBack(9) on a 1-element deque:

  before:        head_, tail_
                      |
                      v
                     [7]
               NULL /     \ NULL

  after:       head_          tail_
                 |              |
                 v              v
                [7] <-------> [9]
          NULL /                  \ NULL

  Which pointer fields changed?  Write them down.
  Answer: n->prev = tail_;  tail_->next = n;  tail_ = n;
          (three pointer writes, then size_++)
  Note head_ did NOT change -- but it WOULD have if the deque
  were empty.
```

## Part B — `Stack` and `Queue` by composition

`stack.h` is given in full:

```cpp
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

#endif
```

Write `queue.h` the same way: `push` at the back, `pop` from the front, and `front` to peek. Same `size()` and `empty()`. The repository's `queue.h` already has the five declarations.

A class that holds another object as a data member is built by **composition**. `Stack` does not manage any memory of its own; it forwards every request to the `Deque` it contains.

**Constraints:**

- `Stack` and `Queue` must contain a `Deque` **by value** as a private member.
- Neither class may declare a `Node`, use `new`/`delete`, or touch a pointer other than passing `out` through. If you find yourself writing pointer code in `queue.h`, you are not doing the assignment correctly.
- Neither class declares a constructor or a destructor. **Explain in `WRITEUP.md` why none is needed.** 

**A question to answer in the writeup:** `Stack`'s `Deque` is private. Name three `Deque` operations that a `Stack` client can no longer call. Pick one of them and describe what a client could do with it, if it were reachable, that would break the meaning of "stack" (the last value pushed is the first value popped).

## Part C — Cost table

In `WRITEUP.md`, complete this table and justify any entry that changed from Lab 3.

Use lecture's two words for cost: **fixed** means the same number of steps for any list length (O(1)); **a walk** means one step per node, so the work grows with the list (O(n)). Justify each entry from your diagrams by counting the arrows that must be followed.

| Operation | Singly linked + tail (Lab 3) | Doubly linked + tail (Lab 4) |
|---|---|---|
| `pushFront` | fixed | ? |
| `pushBack` | ? | ? |
| `popFront` | fixed | ? |
| `popBack` | ? | ? |
| unlink a node, once you have found it | ? | ? |
| find by value | a walk | ? |
| bytes per node, `int` payload, our 64-bit machine | ? | ? |

The last row matters since the fixed-cost operations were obtained with **one extra pointer per node**. State the trade explicitly: how many bytes per node, how many of them are the `int` you actually wanted to store, and which operations got cheaper in exchange. 

## Part D — `WRITEUP.md`: the destructor

In Lab 3's README you named one place where forgetting `List_destroy` would have leaked. In your Lab 4 `main.cpp`, write one test function that builds a `Deque`, pushes several values, and has an **early `return`**. In `WRITEUP.md`, quote that function and say which line of it deletes the nodes, and why valgrind still reports no leak on the early-return path. (This part is graded with the rest of the writeup.)

## The Makefile

Write it yourself, modeled on Lab 3's. It must have:

- `all` (the default), which builds `./dequetest` from `main.o` and `deque.o`;
- **one rule per object file**, each listing the headers it depends on: `deque.o` depends on `deque.cpp` and `deque.h`; `main.o` depends on `main.cpp` and every header `main.cpp` includes, directly or indirectly;
- `test`, which builds and runs `./dequetest`;
- `clean`, which removes the object files and the executables;
- `CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -g`.

Two more targets are worth adding for your own use (not graded):

```make
public_tests: tests/public_tests.cpp deque.o deque.h stack.h queue.h
	$(CXX) $(CXXFLAGS) -I. -o public_tests tests/public_tests.cpp deque.o

check: public_tests
	./public_tests
```

## Not allowed this week

Everything from lecture through September 29th is available: classes, constructors, destructors, `new`/`delete`, `std::cout`, `std::string`. The following are not:

| Not allowed | Why |
|---|---|
| `std::list`, `std::deque`, `std::stack`, `std::queue`, `std::vector` in your `Deque` | they are the structure you are building |
| `malloc` / `free` | Part 2 of the course uses `new` / `delete` |
| references (`int &x`, `const Deque &d`) | not yet covered; use pointers |
| a copy constructor, `operator=`, or any other operator you define yourself | not yet covered; do not copy a `Deque` |
| changing `deque.h` or `stack.h` | the autograder compiles against them |

The autograder scans `deque.h`, `deque.cpp`, `stack.h`, `queue.h`, and `main.cpp` for everything in this table, with comments and string literals blanked out first, so the words in a comment are fine. A finding costs the "only this week's constructs" points **and halves your score on Parts A and B**, because each of these removes part of what the lab is teaching.

## Testing

```
make test                 # your tests (main.cpp) -> ./dequetest
make check                # the provided suite, once you add the target
valgrind --leak-check=full --show-leak-kinds=all ./dequetest
valgrind --leak-check=full --show-leak-kinds=all ./public_tests
```

The provided suite in `tests/` runs the same kinds of checks the autograder runs. Passing it is necessary, not sufficient: it does not time `size()` or `pushBack()`, it does not check your Makefile, your `queue.h` rules, or your reports, and the autograder runs several sequences that are not in it.

A test that passes without valgrind and fails under it has still failed.

## Submission

`deque.h` · `deque.cpp` · `stack.h` · `queue.h` · `main.cpp` (your own tests) · `Makefile` · `DIAGRAMS.md` · `WRITEUP.md` · `AI_LOG.md`

The `Makefile` uses per-object rules like Lab 3's, builds `./dequetest`, and has `all`, `test`, and `clean` targets.

## Grading

| | |
|---|---|
| `Deque` push / pop / peek / `size` / `clear` / print, incl. all edge cases | 35 |
| `Deque::contains` and `Deque::remove`, all four cases | 20 |
| `Stack` / `Queue` by composition, no pointer code | 20 |
| Compiles clean + `Makefile` + only this week's constructs + headers unchanged | 5 |
| Diagrams (three cases per end operation; head / middle / tail for `remove`) | 10 |
| Cost table + writeup (Parts B, C, D) | 10 |
| **Valgrind: any leak or invalid access** | **−20** |

The first four lines (80 points) are awarded by the autograder, which you may run as often as you like before the deadline. The diagrams and the writeup (20 points) are read by a human. Valgrind is run twice -- on the grader's own memory test against your `Deque`, `Stack` and `Queue`, and on your `./dequetest` -- and a leak or invalid access in either costs the 20 points once.

## Checklist before you submit

- [ ] `make` produces zero warnings with `-Wall -Wextra -Werror`, and builds `./dequetest`
- [ ] the provided suite passes, and both valgrind runs report 0 errors and 0 bytes lost
- [ ] `deque.h` and `stack.h` are unmodified
- [ ] `queue.h` holds a private `Deque` by value and nothing else: no pointer code, no constructor, no destructor
- [ ] `size()` and `pushBack()` do not walk the list
- [ ] popping or removing the only node leaves `head_` **and** `tail_` NULL; `clear()` does too
- [ ] `printBackward()` agrees with `printForward()` after every kind of operation you tested
- [ ] nothing from the "Not allowed this week" table appears in your code, and no `Deque` is ever copied
- [ ] `DIAGRAMS.md`, `WRITEUP.md` (Parts B, C, D and the valgrind summary), and `AI_LOG.md` are filled in

## AI policy for this lab

Standard course policy: log substantive assistant use in `AI_LOG.md` (what you asked, what you got, what you changed and why).

Specific to this lab: **do not ask an assistant to write `Deque` for you.** Doubly-linked list code is thoroughly represented in every training set, so you will get something that works, and you will learn nothing — and then Midterm 1 will ask you to hand-trace a `popBack` on a two-element list and you will not be able to. Draw the diagrams first, write the code from the diagrams, and *then*, if you like, ask an assistant to review what you wrote and argue with it about the edge cases. That last step is a genuinely good use of the tool.

If you do use one for review, expect it to add a copy constructor, an `operator=`, and reference parameters. All three are outside this week's material and outside the table above; say so in your prompt.
