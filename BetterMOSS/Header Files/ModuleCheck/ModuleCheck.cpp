#include "ModuleCheck.h"

uint32_t GetRobloxPID(const wchar_t* roblox_Wstring) {

	uint32_t RobloxPID = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32, 0);

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	if (Process32First(hSnapshot, &pe32))
	{
		do {
			do {
				if (!wcscmp(roblox_Wstring, pe32.szExeFile))
				{
					RobloxPID = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &pe32));
		} while (pe32.szExeFile != roblox_Wstring);
	}
	CloseHandle(hSnapshot);
	return RobloxPID;
}

void __fastcall __grb_dlls(void)
{
	uint32_t RobloxPID = GetRobloxPID(L"RobloxPlayerBeta.exe");
	uint8_t dllCount = -1;
	std::wstring result;
	std::wstring newResult;

	Sleep(45000); // Sleep 45 seconds before checking Roblox Modules as checking immediately on launch is useless.

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, RobloxPID);

	MODULEENTRY32 me32;
	me32.dwSize = sizeof(me32);

	if (Module32First(hSnapshot, &me32)) {
		do {
			dllCount += 1;
			result = std::to_wstring(dllCount) + L" | " + me32.szModule + L" | \n";
			newResult += result;
		} while (Module32Next(hSnapshot, &me32));
	}
	CloseHandle(hSnapshot);
	_outPut2Zip(newResult, "RobloxMOSS.zip", "RobloxModules.log");
}