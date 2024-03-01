//
// Floating point support routines
//
#include "msvcrt.h"

#define HI(x)    (*(1 + (int *) &x))
#define LO(x)    (*(int *) &x)
#define HIPTR(x) *(1+(int *) x)
#define LOPTR(x) (*(int *) x)

int _isnan(double x) {
    int hx, lx;

    hx = (HI(x) & 0x7fffffff);
    lx = LO(x);
    hx |= (unsigned) (lx | (-lx)) >> 31;
    hx = 0x7ff00000 - hx;
    return ((unsigned) (hx)) >> 31;
}

double _copysign(double x, double y) {
    HI(x) = (HI(x) & 0x7fffffff) | (HI(y) & 0x80000000);
    return x;
}

int _finite(double x) {
    int hx;
    hx = HI(x);
    return (unsigned) ((hx & 0x7fffffff) - 0x7ff00000) >> 31;
}

unsigned int _control87(unsigned int new, unsigned int mask) {
    // TODO: map between msvcrt fp flag definitions to i387 flags
#if 0
    unsigned int fpcw;

    __asm fnstcw fpcw;
    fpcw = ((fpcw & ~mask) | (new & mask));
    __asm fldcw fpcw;

    return fpcw;
#endif
    return 0;
}

unsigned int _controlfp(unsigned int new, unsigned int mask) {
    syslog(LOG_WARNING, "_controlcp not implemented, ignored");
    return 0;
}
