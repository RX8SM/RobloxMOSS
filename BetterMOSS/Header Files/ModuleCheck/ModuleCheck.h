#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include "..\OutputData\OutputDataH.h"
//Essentially just, get all modules depending on RobloxPlayerBeta.exe
void __fastcall __grb_dlls(void);

//Fundamentally just GetProcessID with a loop till it finds the PID.
uint32_t GetRobloxPID(const wchar_t* roblox_Wstring);