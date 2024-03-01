//
// Port input/output
//
#include <os/krnl.h>

#ifndef VMACH

unsigned char __declspec(naked) inb(port_t port) {
    __asm {
            mov     dx, word ptr[esp + 4]
            xor     eax, eax
            in      al, dx
            ret
    }
}

unsigned short __declspec(naked) inw(port_t port) {
    __asm {
            mov     dx, word ptr[esp + 4]
            xor     eax, eax
            in      ax, dx
            ret
    }
}

unsigned long __declspec(naked) ind(port_t port) {
    __asm {
            mov     dx, word ptr[esp + 4]
            in      eax, dx
            ret
    }
}

void insw(port_t port, void *buf, int count) {
    __asm {
            mov edx, port
            mov edi, buf
            mov ecx, count
            rep insw
    }
}

void insd(port_t port, void *buf, int count) {
    __asm {
            mov edx, port
            mov edi, buf
            mov ecx, count
            rep insd
    }
}

unsigned char __declspec(naked) outb(port_t port, unsigned char val) {
    __asm {
            mov     dx, word ptr[esp + 4]
            mov     al, byte ptr[esp + 8]
            out     dx, al
            ret
    }
}

unsigned short __declspec(naked) outw(port_t port, unsigned short val) {
    __asm {
            mov     dx, word ptr[esp + 4]
            mov     ax, word ptr[esp + 8]
            out     dx, ax
            ret
    }
}

unsigned long __declspec(naked) outd(port_t port, unsigned long val) {
    __asm {
            mov     dx, word ptr[esp + 4]
            mov     eax,[esp + 8]
            out     dx, eax
            ret
    }
}

void outsw(port_t port, void *buf, int count) {
    __asm {
            mov edx, port
            mov esi, buf
            mov ecx, count
            rep outsw
    }
}

void outsd(port_t port, void *buf, int count) {
    __asm {
            mov edx, port
            mov esi, buf
            mov ecx, count
            rep outsd
    }
}

#endif
