#include <windows.h>
#include <winternl.h>
#include <stdio.h>

#pragma comment(lib, "ntdll.lib")

static BOOL UsingPEB() {
    PEB* peb = (PEB*)__readgsqword(0x60);
    return peb->BeingDebugged;
}

static BOOL UsingCheckRemoteDebuggerPresent() {
    BOOL isDebuggerPresent = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent);
    return isDebuggerPresent;
}

static BOOL UsingIsDebuggerPresent() {
    return IsDebuggerPresent();
}

static BOOL UsingNtQueryInformationProcessDebugPort() {
    DWORD debugFlag = 0;
    NtQueryInformationProcess(GetCurrentProcess(), ProcessDebugPort, &debugFlag, sizeof(debugFlag), NULL);
    return debugFlag != 0;
}

static BOOL UsingNtQueryInformationProcessDebugFlags() {
    DWORD debugFlag = 0;
    NtQueryInformationProcess(GetCurrentProcess(), ProcessDebugFlags, &debugFlag, sizeof(debugFlag), NULL);
    return debugFlag == 0;
}

int CheckDebugger(void) {
    // Check using all methods
    if (UsingPEB() || 
        UsingCheckRemoteDebuggerPresent() || 
        UsingIsDebuggerPresent() || 
        UsingNtQueryInformationProcessDebugPort() || 
        UsingNtQueryInformationProcessDebugFlags()) {
        return 1;
    }
    return 0;
}
