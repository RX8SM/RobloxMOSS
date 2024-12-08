#include "ProcLogger.h"

#pragma comment(lib, "crypt32.lib")

std::string __gt_SHA256(const std::wstring& filePath) {
    HCRYPTPROV hProv = NULL;
    HCRYPTHASH hHash = NULL;
    BYTE hash[32];  // SHA-256 produces a 256-bit (32-byte) hash
    DWORD hashSize = sizeof(hash);
    std::string hashString;

    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        // std::cerr << "Failed to open file. Code: " << GetLastError() << std::endl;
        return "";
    }

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "CryptAcquireContext failed. Code: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return "";
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        std::cerr << "CryptCreateHash failed. Code: " << GetLastError() << std::endl;
        CryptReleaseContext(hProv, 0);
        CloseHandle(hFile);
        return "";
    }

    const int bufferSize = 1024; //1kb
    BYTE buffer[bufferSize];
    DWORD bytesRead;
    while (ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
        if (!CryptHashData(hHash, buffer, bytesRead, 0)) {
            std::cerr << "CryptHashData failed. Code: " << GetLastError() << std::endl;
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            CloseHandle(hFile);
            return "";
        }
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0)) {
        std::cerr << "CryptGetHashParam failed. Code: " << GetLastError() << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        CloseHandle(hFile);
        return "";
    }

    // hash to hex conversion
    for (DWORD i = 0; i < hashSize; i++) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", hash[i]);
        hashString += buf;
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);

    return hashString;
}

inline const wchar_t* __get_fP(uint32_t PID) {
    wchar_t buffer[MAX_PATH] = { 0 };

    HANDLE fileHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, PID);
    if (fileHandle == NULL) {
        // std::wcerr << L"Failed to open process " << PID << L". Code: " << GetLastError() << std::endl;
        return L"";
    }

    if (GetModuleFileNameExW(fileHandle, NULL, buffer, _countof(buffer)) == 0) {
        // std::wcerr << L"Failed to get file path for PID: " << PID << L". Code: " << GetLastError() << std::endl;
        CloseHandle(fileHandle);
        return L"";
    }

    CloseHandle(fileHandle);

    return buffer;
}

void __fastcall logProcesses(void) {

       std::set<uint32_t> seenPIDs; 

        while (true) {
            HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hSnapshot == INVALID_HANDLE_VALUE) {
                std::cerr << "CreateToolhelp32Snapshot failed. Code: " << GetLastError() << std::endl;
                return;
            }

            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof(pe32);


            if (Process32First(hSnapshot, &pe32)) {
                do {
                    uint32_t PID = pe32.th32ProcessID;
                    std::wstring szExe = pe32.szExeFile;
                    uint32_t RobloxPID = 0;
                    std::wstring GameDetection;
                    if (seenPIDs.find(PID) == seenPIDs.end()) {
                        if (!wcscmp(L"RobloxPlayerBeta.exe", pe32.szExeFile)) {
                            GameDetection = L"\n\n\n\n\n\n\nGame detected.\n\n\n\n\n\n\n\n";
                            RobloxPID = PID;
                        }
                        
                        auto now = std::chrono::system_clock::now();
                        
                        std::wostringstream timeWO;
                        std::wstring timeW;
                        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                        std::tm localTime;
                        // Keep track of time when processes are logged.
                        if (localtime_s(&localTime, &currentTime) == 0) {
                            timeWO << std::put_time(&localTime, L"%Y-%m-%d %H:%M:%S | ");
                            timeW = timeWO.str();
                        }
                        else {
                            std::wcerr << L"Failed to get local time" << std::endl;
                        }

                        const wchar_t* filePath = __get_fP(PID);
                        std::string SHA256Hash = __gt_SHA256(filePath);
                        std::wstring signatureResult = VerifyDigitalSignature(filePath);



                        std::wstring processData = L"PID: " + std::to_wstring(PID) + L" | File Path: " + filePath + L" | " + szExe + L"\n";
                        std::string hashData = "File hash: " + SHA256Hash;
                        std::wstring wHashData(hashData.begin(), hashData.end());
                        std::wstring Data = wHashData + L" | " + timeW + signatureResult + processData + GameDetection;
                        

                        std::wstring newData = L""; 
                        newData += Data;

                        const char* zipName = "RobloxMOSS.zip";
                        const char* fileName = "Processes.log";
                         
                        _outPut2Zip(newData, zipName, fileName);

                        seenPIDs.insert(PID);
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    }

                } while (Process32Next(hSnapshot, &pe32)); 
            }
            else {
                std::cerr << "Process32First failed. Code: " << GetLastError() << std::endl;
            }


            CloseHandle(hSnapshot);

   
        }     
}
