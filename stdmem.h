#ifndef STDMEM_H
#define STDMEM_H

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>
    #include <unistd.h>
    #include <sys/mman.h>
#endif

#define MEM_ALIGN 64

static inline void* memAlloc(size_t size) {
    void* ptr = NULL;
#if defined(_WIN32)
    ptr = _aligned_malloc(size, MEM_ALIGN);
#else
    if (posix_memalign(&ptr, MEM_ALIGN, size) != 0) {
        return NULL;
    }
#endif
    return ptr;
}

static inline void* memZero(size_t size) {
    void* ptr = memAlloc(size);
    if (ptr) {
        __builtin_memset(ptr, 0, size);
    }
    return ptr;
}

static inline void* memResize(void* ptr, size_t newSize) {
#if defined(_WIN32)
    return _aligned_realloc(ptr, newSize);
#else
    void* newPtr = memAlloc(newSize);
    if (!newPtr) {
        return NULL;
    }
    __builtin_memcpy(newPtr, ptr, newSize);
    free(ptr);
    return newPtr;
#endif
}

static inline void memFree(void* ptr) {
#if defined(_WIN32)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

static inline void* memClone(const void* src, size_t size) {
    void* dst = memAlloc(size);
    if (dst) {
        __builtin_memcpy(dst, src, size);
    }
    return dst;
}

static inline void* memMap(size_t size) {
#if defined(_WIN32)
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
}

static inline void memUnmap(void* ptr, size_t size) {
#if defined(_WIN32)
    VirtualFree(ptr, 0, MEM_RELEASE);
#else
    munmap(ptr, size);
#endif
}

#endif // STDMEM_H
