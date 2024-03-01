//
// Non-local goto
//
#include <signal.h>
#include <setjmp.h>

#define OFS_EBP   0
#define OFS_EBX   4
#define OFS_EDI   8
#define OFS_ESI   12
#define OFS_ESP   16
#define OFS_EIP   20

__declspec(naked) int setjmp(jmp_buf env) {
    __asm {
            mov edx, 4[esp]          // Get jmp_buf pointer
            mov eax,[esp]           // Save EIP
            mov OFS_EIP[edx], eax
            mov OFS_EBP[edx], ebp    // Save EBP, EBX, EDI, ESI, and ESP
            mov OFS_EBX[edx], ebx
            mov OFS_EDI[edx], edi
            mov OFS_ESI[edx], esi
            mov OFS_ESP[edx], esp
            xor eax, eax             // Return 0
            ret
    }
}

__declspec(naked) void longjmp(jmp_buf env, int value) {
    __asm {
            mov edx, 4[esp]          // Get jmp_buf pointer
            mov eax, 8[esp]          // Get return value (eax)

            mov esp, OFS_ESP[edx]    // Switch to new stack position
            mov ebx, OFS_EIP[edx]    // Get new EIP value and set as return address
            mov[esp], ebx

            mov ebp, OFS_EBP[edx]    // Restore EBP, EBX, EDI, and ESI
            mov ebx, OFS_EBX[edx]
            mov edi, OFS_EDI[edx]
            mov esi, OFS_ESI[edx]

            ret
    }
}

int sigsetjmp(sigjmp_buf env, int savesigs) {
    if (savesigs) {
        sigprocmask(SIG_BLOCK, NULL, &env->sigmask);
    } else {
        env->sigmask = -1;
    }
    return setjmp(env->env);
}

void siglongjmp(sigjmp_buf env, int value) {
    if (env->sigmask != -1) {
        sigprocmask(SIG_SETMASK, &env->sigmask, NULL);
    }
    longjmp(env->env, value);
}
