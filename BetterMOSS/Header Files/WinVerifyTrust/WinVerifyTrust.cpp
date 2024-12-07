#include "WinVerifyTrust.h"

std::wstring VerifyDigitalSignature(const wchar_t* filePath)
{
    WINTRUST_FILE_INFO fileInfo = { 0 };
    fileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
    fileInfo.pcwszFilePath = filePath;
    fileInfo.hFile = NULL;
    fileInfo.pgKnownSubject = NULL;

    const GUID WINTRUST_ACTION_GENERIC_VERIFY_V2 =
    { 0xaac56b, 0xcd44, 0x11d0, { 0x8c, 0xc2, 0x0, 0xc0, 0x4f, 0xc2, 0x95, 0xee } };

    GUID actionGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;

    WINTRUST_DATA trustData = { 0 };
    trustData.cbStruct = sizeof(WINTRUST_DATA);
    trustData.dwUIChoice = WTD_UI_NONE;            
    trustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    trustData.dwUnionChoice = WTD_CHOICE_FILE;
    trustData.pFile = &fileInfo;
    trustData.dwStateAction = WTD_STATEACTION_VERIFY;

    LONG result = WinVerifyTrust(NULL, &actionGUID, &trustData);

    trustData.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(NULL, &actionGUID, &trustData);

    if (result == ERROR_SUCCESS)
    {
        std::wstring signatureResult = L"VALID SIGNATURE | ";

        return signatureResult;
    }
    else if (result == TRUST_E_NOSIGNATURE)
    {
        std::wstring signatureResult = L"NOT SIGNED | ";
        
        return signatureResult;
    }
    else if (result == TRUST_E_BAD_DIGEST)
    {
        std::wstring signatureResult = L"INVALID SIGNATURE | ";
        return signatureResult;
    }
    else
    {
        std::wstring signatureResult = L"ERROR OCCURED | ";
        return signatureResult;
    }

    return L"";
}
