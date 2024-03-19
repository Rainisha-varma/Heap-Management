# Heap-Management

This code provides a simple custom memory allocator in C.

Features:

    Manages memory dynamically.
    First-fit allocation strategy.
    Tracks memory usage with metadata.
    Splits blocks for better fit.
    Merges free blocks for efficiency.

Usage:

    Include <stdio.h> and <stdlib.h>.
    Define HEAP_SIZE (total memory to manage).
    Call initialize to create the allocator.
    Use Malloc to allocate memory of specific sizes and types.
    Use Free to release memory when done.
    (Optional) Use Displayfree to view available free blocks.
    Call free on the initialized memory block to release the entire heap.

