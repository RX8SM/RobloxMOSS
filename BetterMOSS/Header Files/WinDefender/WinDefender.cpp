#include "WinDefender.h"

std::wstring __fastcall IsRealTimeProtectionEnabled()
{
    std::wostringstream timeWO;
    std::wstring timeW;
    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime;

    if (localtime_s(&localTime, &currentTime) == 0) {
        timeWO << std::put_time(&localTime, L"%Y-%m-%d %H:%M:%S | ");
        timeW = timeWO.str();
    }
    else {
        std::wcerr << L"Failed to get local time" << std::endl;
    }

    std::wstring status;

    const char* regPath = "SOFTWARE\\Microsoft\\Windows Defender\\Real-Time Protection";
    HKEY hKey;
    LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey);

    if (result != ERROR_SUCCESS)
    {
        std::wcout << L"RegOpenKeyExA failed. Code: " << result << std::endl;
        status = L"Error accessing registry.\n";
        return status;
    }

    DWORD value = 0;
    DWORD valueSize = sizeof(value);

    result = RegQueryValueExA(hKey, "DisableRealtimeMonitoring", 0, NULL, (BYTE*)&value, &valueSize);

    if (result != ERROR_SUCCESS)
    {
        if (result == 2) // If the error code is 2, the subkey doesn't exist. Windows Defender only spawns a subkey when it is disabled and initializes Its value to 1.
        {
            status = L"Windows Defender is enabled.\n";
        }
        else
        {
            status = L"RegQueryValueExA failed. Code: " + std::to_wstring(result) + L"\n";
        }
        RegCloseKey(hKey);
        return timeW + status;
    }

    if (value == 1) {
        status = L"Windows Defender is disabled.\n";
    }
    else {
        status = L"Windows Defender is enabled.\n";
    }

    status = timeW + status;

    RegCloseKey(hKey);

    return status;
}

    