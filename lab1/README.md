# Dynamic Memory and String Utilities

This project implements a lightweight C++ utility layer for dynamic string handling and integer array management. It focuses on manual memory allocation, safe resizing, and careful lifecycle control, with logic designed to preserve valid state across growth and failure scenarios.

The solution includes custom string operations and a dynamically resizing integer array built without relying on higher-level container abstractions. The code is structured around pointer correctness, boundary checks, and cleanup paths that prevent leaks and invalid memory access.

The project also includes a driver for exercising edge cases, verifying expected behavior, and validating correctness under strict compiler and memory-analysis checks.
