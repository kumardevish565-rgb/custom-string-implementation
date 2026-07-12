# Custom C++ String Implementation

A personal project built to understand low-level memory management, pointers, and object lifecycles in C++ by creating a custom string class from scratch. This project mimics the fundamental functionality and internal workings of `std::string` to demonstrate how string engines manage dynamic buffers under the hood.

## Project Structure

The repository contains two distinct implementations of the custom string class:
*   `c_string.cpp`: Focuses on manual memory allocation using `malloc`, `realloc`, and `free`, alongside standard `<cstring>` functions like `strcpy`.
*   `cpp_string.cpp`: A refactored version using idiomatic C++ memory management (`new[]`/`delete[]`), explicit Rule of 5 handling, and C++ Standard Library algorithms (`std::copy`, `std::copy_backward`).

Both files include their own built-in high-resolution benchmarking routines inside `main()`.


## Architectural Comparison

### Similarities
*   **API Design:** Both versions mimic `std::string` functionality by implementing matching member functions and operator overloads, including deep-copy constructors, assignment operators, character access (`[]`), and concatenation (`+=`).
*   **Custom Core Logic:** All foundational algorithms—including index tracking, null terminator (`\0`) bounds enforcement, character insertion, and the substring searching (`find`) routines—are entirely my own custom logic written from scratch without relying on standard utility wrappers.

### Differences
*   **Memory Primitives:** `c_string.cpp` manages the heap via C standard library calls (`malloc`/`realloc`/`free`), whereas `cpp_string.cpp` leverages native C++ expressions (`new[]`/`delete[]`).
*   **Data Transfer:** The C-style code relies on null-terminated character scans via `<cstring>` functions like `strcpy`. The C++ version refactors this to use bounded data ranges via `<algorithm>` primitives (`std::copy` and `std::copy_backward`).
*   

## Performance Benchmarks

To evaluate the efficiency of both custom architectures, the implementations were benchmarked against the official standard library (`std::string`) over 100,000+ operations. 

### Deep Copying / Assignment Operator Results (Compiled with `-O2`)
*  Modern C++ Version (cpp_string.cpp): ~0.66 ms (Average)
*  C-Style Version (c_string.cpp): ~0.85 ms (Average)
*  Standard Library (std::string): ~2.13 ms (Average)

### Key Findings
*   **The Power of `std::copy`:** When compiled with optimizations (`-O2`), the modern C++ implementation performed the fastest. Because it tracks string length explicitly, the compiler optimizes `std::copy` into a raw hardware-level block memory transfer.
*   **The Limitation of `strcpy`:** The C-style implementation is held back because `strcpy` is forced to check every single byte for the null terminator (`\0`), preventing the CPU from maximizing execution speed.
*   **Standard Library Overhead:** While `std::string` is highly optimized, its copy assignment operator carries broad internal bookkeeping overhead (handling Small String Optimization checks and allocator states) that makes a lean, single-purpose implementation faster in raw, isolated deep-copy tests.


## How to Compile and Run

You can replicate these benchmarks using any standard C++ compiler. Ensure you enable optimization flags (`-O2` or `-O3`) to see the true performance capabilities of the code.

```bash
# Compile and run the C-style version
g++ -O2 c_string.cpp -o c_benchmark
./c_benchmark

# Compile and run the modern C++ version
g++ -O2 cpp_string.cpp -o cpp_benchmark
./cpp_benchmark
