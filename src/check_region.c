#include <windows.h>
#include <stdio.h>

int CheckRegion(void) {
    GEOID geoId = GetUserGeoID(GEOCLASS_NATION);
    if (geoId == GEOID_NOT_AVAILABLE) {
        printf("Unable to retrieve the region information.\n");
        return 0;
    }

    WCHAR regionName[100];
    int result = GetGeoInfoW(geoId, GEO_FRIENDLYNAME, regionName, sizeof(regionName) / sizeof(WCHAR), 0);
    if (result == 0) {
        printf("Unable to retrieve the region name.\n");
        return 0;
    }

    // Compare with "France" (case-insensitive)
    WCHAR france[] = L"France";
    if (_wcsicmp(regionName, france) != 0) {
        printf("This program can only run in France.\n");
        return 0;
    }

    return 1;
}
