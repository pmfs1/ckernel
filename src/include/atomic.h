//
// Atomic operations
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef ATOMIC_H
#define ATOMIC_H

#ifdef  __cplusplus
extern "C" {
#endif

// On uniprocessors, the 'lock' prefixes are not necessary (and expensive). 
// Since ckernel does not (yet) support SMP the 'lock' prefix is disabled for now.

#pragma warning(disable: 4035) // Disables warnings reporting missing return statement

__inline int atomic_add(int *dest, int value) {
    __asm {
            mov edx, dest;
            mov eax, value;
            mov ecx, eax;
        /*lock*/ xadd dword ptr[edx], eax;
            add eax, ecx;
    }
    }

    __inline int atomic_increment(int *dest) {
        __asm {
                mov edx, dest;
                mov eax, 1;
            /*lock*/ xadd dword ptr[edx], eax;
                inc eax;
        }
        }

    __inline int atomic_decrement(int *dest) {
        __asm {
                mov edx, dest;
                mov eax, -1;
            /*lock*/ xadd dword ptr[edx], eax;
                dec eax;
        }
        }

    __inline int atomic_exchange(int *dest, int value) {
        __asm {
                mov eax, value;
                mov ecx, dest;
                xchg eax, dword ptr[ecx];
        }
        }

    __inline int atomic_compare_and_exchange(int *dest, int exchange, int comperand) {
        __asm {
                mov edx, dest
                mov ecx, exchange
                mov eax, comperand
            /*lock*/ cmpxchg dword ptr[edx], ecx
        }
    }

#pragma warning(default: 4035)

#ifdef  __cplusplus
    }
#endif

#endif
