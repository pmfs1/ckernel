//
// Bitmap manipulation routines
//
#ifndef BITOPS_H
#define BITOPS_H

#ifdef  __cplusplus
extern "C" {
#endif

__inline void set_bit(void *bitmap, int pos) {
    __asm {
            mov eax, pos
            mov ebx, bitmap
            bts dword ptr[ebx], eax
    }
}

__inline void clear_bit(void *bitmap, int pos) {
    __asm {
            mov eax, pos
            mov ebx, bitmap
            btr dword ptr[ebx], eax
    }
}

__inline int test_bit(void *bitmap, int pos) {
    int result;

    __asm {
            mov eax, pos
            mov ebx, bitmap
            bt dword ptr[ebx], eax
            sbb eax, eax
            mov result, eax
    }

    return result;
}

static __inline int find_lowest_bit(unsigned mask) {
    int n;

    __asm {
            bsf eax, mask
            mov n, eax
    }

    return n;
}

#if 0
static __inline int find_highest_bit(unsigned mask) {
  int n;

  __asm {
    bsr eax, mask
    mov n, eax
  }

  return n;
}
#else

static __inline int find_highest_bit(unsigned mask) {
    int n = 31;

    while (n > 0 && !(mask & 0x80000000)) {
        mask <<= 1;
        n--;
    }

    return n;
}

#endif

__inline void set_bits(void *bitmap, int pos, int len) {
    while (len-- > 0) set_bit(bitmap, pos++);
}

int find_first_zero_bit(void *bitmap, int len);

int find_next_zero_bit(void *bitmap, int len, int start);

#ifdef  __cplusplus
}
#endif

#endif
