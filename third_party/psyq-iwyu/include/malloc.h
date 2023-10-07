/*
 * File:malloc.h
 */
/*
 * $PSLibId: Run-time Library Release 4.7$
 */
#ifndef _MALLOC_H
#define _MALLOC_H

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;  /* result type of the sizeof operator (ANSI) */
#endif
#ifndef NULL
#define NULL 0		       /* null pointer constant */
#endif

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
extern void InitHeap (unsigned long *, unsigned long);
extern void free(void *);
extern void *malloc(size_t);
extern void *calloc(size_t, size_t);
extern void *realloc(void *, size_t);
extern void InitHeap2 (unsigned long *, unsigned long);
extern void free2(void *);
extern void *malloc2(size_t);
extern void *calloc2(size_t, size_t);
extern void *realloc2(void *, size_t);
/**
 * Initializes a heap area of size bytes. If size is not divisible by 8, the remainder after dividing by 8 is
 * discarded and isn’t allocated. (Since there is overhead, the entire size in bytes cannot be used.)
 * After calling this function, the library memory routines in the “malloc3” group (malloc3(), free3(), etc.) are
 * usable. This function is a higher speed than the “malloc2” system and is smaller in size. See "Memory
 * Allocation Functions" in the Kernel chapter of the Library Overview for more information on the malloc
 * systems.
 * If several executions of this function overlap, the previous memory control information is lost.
 * @param head Pointer to heap start address
 * @param size Heap size (a multiple of 8, in bytes)
 */
extern void InitHeap3(unsigned long *head, unsigned long size);
extern void free3(void *);

/**
 * Allocates s bytes of memory block from the heap memory. InitHeap3() must be executed in advance.
 * Refer to the section entitled "Memory Allocation Functions" in the Kernel chapter of the Library Overview for
 * the differences between the various malloc systems.
 * @params s Size of memory block to be allocated
 * @return A pointer to the allocated memory block. If allocation fails, NULL is returned.
 */
extern void *malloc3(size_t s);
/**
 * Allocates a block of n*s bytes in the heap memory and initializes it to 0. Corresponds to InitHeap3().
 * @param n Number of partitions
 * @param s Size of one partition
 * @return A pointer to the allocated memory block. If allocation fails, NULL is returned.
 */
extern void *calloc3(size_t n, size_t s);
extern void *realloc3(void *, size_t);
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* _MALLOC_H */

