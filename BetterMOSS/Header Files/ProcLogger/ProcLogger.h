// Function prototypes and includes.

#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <set>
#include <wincrypt.h>
#include <chrono>
#include <thread>
#include "..\WinVerifyTrust\WinVerifyTrust.h"
#include "..\WinDefender\WinDefender.h"
#include "..\ModuleCheck\ModuleCheck.h"
#include "..\OutputData\OutputDataH.h"

// Provides SHA256 of a file, based off the filePath provided.
std::string __gt_SHA256(const std::wstring& filePath);

// Grabs a file path based on the PID provided.
inline const wchar_t* __get_fP(uint32_t PID);

// Grabs PID's from all running processes and prints out when Roblox was ran along with some other additional information.
void __fastcall logProcesses(void);
