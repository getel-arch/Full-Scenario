#include <windows.h>
#include <stdio.h>

static int CheckService(SC_HANDLE hSCManager, const char* serviceName) {
    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_QUERY_STATUS);
    if (hService == NULL) {
        return 2;  // Service not found
    }

    SERVICE_STATUS_PROCESS ssp;
    DWORD bytesNeeded;
    int result = 1;
    
    if (QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssp, sizeof(ssp), &bytesNeeded)) {
        if (ssp.dwCurrentState == SERVICE_RUNNING) {
            result = 0;
        } else {
            printf("%s service is not running.\n", serviceName);
        }
    } else {
        printf("Failed to query service status for %s. Error: %lu\n", serviceName, GetLastError());
        result = 2;
    }

    CloseServiceHandle(hService);
    return result;
}

int CheckSysmonStatus(void) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCManager == NULL) {
        printf("Failed to open Service Control Manager. Error: %lu\n", GetLastError());
        return 1;
    }

    int sysmon64Result = CheckService(hSCManager, "Sysmon64");
    int sysmonResult = CheckService(hSCManager, "Sysmon");

    CloseServiceHandle(hSCManager);
    
    // If both services are not found (result = 2), print the message
    if (sysmon64Result == 2 && sysmonResult == 2) {
        printf("Sysmon service not found\n");
    }
    
    // Return success if at least one service is running
    return (sysmon64Result && sysmonResult);
}
