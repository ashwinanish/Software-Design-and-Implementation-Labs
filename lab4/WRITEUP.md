# Lab 4 writeup -- Ashwin Anish, AA224496

## Part B -- Stack and Queue

**Why neither class needs a constructor or a destructor.**

The stack and queue do not own any nodes directly; it contains a Deque. The constructor for Deque is called when created and the Deque destructor is called when destroyed. 

**What a Stack client cannot call.**

A stack client cannot call pushBack, popBack, or back because those would expose the wrong end of the deque. If a Stack client could call pushBack, they could add values to the back and then pop from the front, which would let the oldest value leave first and break the rule that the last value pushed is the first value popped.

## Part C -- cost table

**fixed** = the same number of steps for any list length; **a walk** = one step per node.

| Operation | Singly linked + tail (Lab 3) | Doubly linked + tail (Lab 4) |
|---|---|---|
| `pushFront` | fixed | fixed |
| `pushBack` | fixed | fixed |
| `popFront` | fixed | fixed |
| `popBack` | a walk | fixed |
| unlink a node, once you have found it | a walk | fixed |
| find by value | a walk | a walk |
| bytes per node, `int` payload, our 64-bit machine | 16 bytes per node | 24 bytes per node |

Pop back is a walk in a singly linked list because there is no prev poitner, so the code must walk from head to the second-to-last node. In a dobly linked list, tail gives direct access to the last node so it is fixed. 

Unlinking a node after finding it is a walk in a singly linked list because you need the predeceessor, but in a doubly linked list, the node already knows both neighbors so you just hae to rewire the two neighbors and update the head and tail as needed. 

On a 64-bit machine, a node in the doubly linked list holds: one int (4 bytes) + one prev pointer (8 bytes) + one next pointer (8 bytes) = 20 bytes before padding. Because of 8-byte alignment, the actual node size is 24 bytes. The extra prev pointer is the tradeoff that makes pushBack, popBack, and unlinking constant-time.


## Part D -- the destructor

```cpp
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
```

The nodes are deleted when the local variable d goes out of scope at the end of the function. Valgrind reports no leak because when d goes out of scope, the Destructor for deques runs and deletes d which frees all the related memory. 


## valgrind

```
==3017227== HEAP SUMMARY:
==3017227==     in use at exit: 0 bytes in 0 blocks
==3017227==   total heap usage: 1,050 allocs, 1,050 frees, 98,880 bytes allocated
==3017227==
==3017227== All heap blocks were freed -- no leaks are possible
==3017227==
==3017227== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
