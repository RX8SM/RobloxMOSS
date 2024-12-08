#include "Header Files/OutputData/OutputDataH.h"
#include "Header Files/ProcLogger/ProcLogger.h"
#include "Header Files/WinDefender/WinDefender.h"
#include "Header Files/ModuleCheck/ModuleCheck.h"
#include <thread>

void __fastcall Start(void)
{
    // Seperate threads for different functions so we can run multiple loops simultaneously.
    std::thread logProcess(logProcesses); 
    std::thread logRobloxDlls(__grb_dlls);

    while (true) {
        std::wstring status = IsRealTimeProtectionEnabled();
        _outPut2Zip(status, "RobloxMOSS.zip", "WindowsDefender.log");
        Sleep(60000);
    } // Check every 60 seconds the state of Windows Defender.
}

int main() {
    
                 MessageBoxA(NULL, "Just a thing I made for fun and I was bored, not sure what else I would add that'd be useful for Roblox. Most of MOSS's current features are useless and unreliable. It is extremely laggy so I decided to remake it.", "RobloxMOSS", MB_OK | MB_ICONASTERISK);
    __int8 msg = MessageBoxA(NULL, "This program is completely safe and is fully open-sourced on github and requires no network connection. Do you agree on running this program?", "RobloxMOSS", MB_YESNO | MB_ICONQUESTION);

    
    if (msg == IDYES) {
        std::cout << "Scanning..." << std::endl;
        std::cout << "If you want to stop scanning just close this program." << std::endl;
        std::cout << "Errors occuring for system files in logs is normal." << std::endl; 
        Start();
    }

    else {
        MessageBoxA(NULL, "Exitted the program, nothing was executed.", "RoblosMOSS", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    return 0;
}