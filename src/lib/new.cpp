//
// Object new and delete operators
//
#include <os.h>

extern "C"
{

void *memset(void *, int, size_t);

int _purecall() {
    panic("pure virtual function call attempted");
    return 0;
}

}

void *operator new(unsigned int size) {
    void *p;

    p = malloc(size);
    if (p) memset(p, 0, size);
    return p;
}

void operator delete(void *p) {
    if (p) free(p);
}
