#include <stdio.h>
#include <unload_sysmon.h>
#include <patch_etw.h>

int main(void) {
    int result;

    printf("Unloading Sysmon...\n");
    result = UnloadSysmon();
    if (result != 0) {
        printf("Failed to unload Sysmon\n");
        return result;
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