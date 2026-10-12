# Deque, Stack, and Queue Implementation

This project builds a doubly linked deque in C++ and uses it as the foundation for both stack and queue interfaces. The implementation is designed around efficient front- and back-end operations while preserving correct node ownership and predictable behavior across edge cases.

The deque supports insertion, removal, peeking, and traversal, and the stack and queue layers are implemented by composing that shared structure instead of duplicating logic. This keeps the API simple while making the underlying behavior consistent and easier to reason about.

The project focuses on robust pointer management, clear ownership boundaries, and a reusable data-structure design that can be adapted for different access patterns without changing the core structure.
