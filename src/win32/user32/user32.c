//
// Win32 USER32 emulation
//
#include <os.h>
#include <sys/types.h>
#include <win32.h>

int WINAPI
MessageBoxA(
        HWND
hWnd,
LPCTSTR lpText,
        LPCTSTR
lpCaption,
UINT uType
) {
TRACE("MessageBoxA");
syslog(LOG_INFO,
"Messagebox %s: %s", lpCaption, lpText);
panic("MessageBoxA not implemented");
return 0;
}

int __stdcall DllMain(handle_t hmod, int reason, void *reserved) {
    return TRUE;
}
