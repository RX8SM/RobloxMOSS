#pragma once
#include <windows.h>
#include <wintrust.h>
#include <tchar.h>
#include <iostream>
#include <strsafe.h>

#include "..\OutputData\OutPutDataH.h"

#pragma comment(lib, "wintrust")
#pragma comment(lib, "crypt32")

// Uses WinVerifyTrust to verify digital signatures of files.
std::wstring VerifyDigitalSignature(const wchar_t* filePath);
