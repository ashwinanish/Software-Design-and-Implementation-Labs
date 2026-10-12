# Record Storage and Database Abstraction

This project implements a lightweight record-based database in C++ with a clean public interface and internal implementation details kept separate. It manages dynamic storage for records, validates inputs, and preserves safe ownership across creation, modification, and teardown.

The design focuses on encapsulation and predictable behavior, with support for both valid and malformed record files to confirm the system responds correctly under real-world conditions. Memory allocation and cleanup are handled explicitly to maintain stable runtime behavior and avoid leaks or invalid access.

The result is a compact database abstraction that demonstrates disciplined data structure design in a lower-level language while keeping the external API straightforward.
