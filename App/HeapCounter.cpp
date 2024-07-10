#if _DEBUG
#define HEAP_SIZE_MAX 9999
#include <cstdlib>
#include "WinServer.h"

unsigned long long& heapSize();

struct _voidPtr {
    void* ptr;
    size_t size;
};

union __ptrUnion {
    _voidPtr a;
    size_t b[2];
};

__ptrUnion* heapVec();

void heapDump() {
    size_t total = 0;

    for (size_t i = 0; i < heapSize(); i++) {
        if (heapVec()[i].a.ptr) {
            free(heapVec()[i].a.ptr);

            Err(
                "[Leaked] {" << heapVec()[i].a.ptr
                << "} has not cleaned: " << heapVec()[i].a.size
                << "\n"
            );
            total += heapVec()[i].a.size;
        }
    }

    Err("total: " << total << " bytes has leaked.\n");
    LogDump();
}

__ptrUnion* heapVec() {
    static __ptrUnion r[HEAP_SIZE_MAX] = { 0, };
    return r;
}

unsigned long long& heapSize() {
    static unsigned long long r = 0;
    return r;
}

void* operator new(size_t size) {
    for (size_t i = 0; i < heapSize(); i++) {
        if (!heapVec()[i].a.ptr) {
            void* r = malloc(size);
            if (!r) return 0;

            heapVec()[i] = { r, size };

            return (r);
        }
    }

    if (heapSize() == HEAP_SIZE_MAX) {
        std::exit(1);
        return 0;
    }

    void* r = malloc(size);
    if (!r) return 0;
    heapVec()[heapSize()++] = { r, size };
    return r;
}

void operator delete(void* ptr) noexcept {
    free(ptr);
    for (size_t i = 0; i < heapSize(); i++) {
        if (ptr == heapVec()[i].a.ptr) {
            heapVec()[i].a.ptr = 0;
            if (i == heapSize() - 1) {
                heapSize()--;
            }
            return;
        }
    }
}
#endif // DEBUG
