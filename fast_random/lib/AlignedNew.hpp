/* Usage:
 *
 *     struct MyData {
 *         int a;
 *         float b;
 *         int stuff[3];
 *
 *         DYNAMIC_ALIGNMENT(MyData);
 *     };
 *
 *     ...
 *
 *     MyData* data = new (256) MyData;
 *
 *     ...
 *
 *     assert(IsAligned(data, 256));
 *     delete data4;
 */
#include <cstdlib>
#include <cassert>


inline void* aligned_malloc(size_t size, int alignment) {
    const int pointerSize = sizeof(void*);
    const int requestedSize = size + alignment - 1 + pointerSize;
    void* raw = std::malloc(requestedSize);
    void* start = (char*)raw + pointerSize;
    void* aligned = (void*)(((unsigned int)((char*)start+alignment-1)) & ~(alignment-1));
    *(void**)((char*)aligned-pointerSize) = raw;
    return aligned;
}

inline void aligned_free(void* aligned) {
    void* raw = *(void**)((char*)aligned-sizeof(void*));
    std::free(raw);
}


inline bool IsAligned(void* data, int alignment) {
    assert((alignment & (alignment-1)) == 0);
    return ((uintptr_t)data & (alignment-1)) == 0;
}


#define DYNAMIC_ALIGNMENT(T) \
    void* operator new(size_t size) \
    { \
        return aligned_malloc(size, __alignof(T)); \
    } \
    void* operator new(size_t size, int alignment) \
    { \
        return aligned_malloc(size, alignment); \
    } \
    void* operator new[](size_t size) \
    { \
        return aligned_malloc(size, __alignof(T)); \
    } \
    void* operator new[](size_t size, int alignment) \
    { \
        return aligned_malloc(size, alignment); \
    } \
    void operator delete(void* p) \
    { \
        aligned_free(p); \
    } \
    void operator delete[](void* p) \
    { \
        aligned_free(p); \
    }
