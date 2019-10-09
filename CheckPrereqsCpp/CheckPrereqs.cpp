// AddDllDirectoryDetect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>

int main()
{
    auto hModule = LoadLibrary(L"kernel32.dll"); // Dont' use LoadLibraryEx here
    FARPROC hFarProc = nullptr;
    bool kb2533623Exists = false;
    if (hModule != nullptr)
    {
        hFarProc = GetProcAddress(hModule, "AddDllDirectory");
        kb2533623Exists = (hFarProc != nullptr);
        FreeLibrary(hModule);
        hModule = nullptr;
    }


    if (kb2533623Exists)
    {
        std::wcout << L"Either running on Win8+, or KB2533623 is installed" << std::endl;
    }
    else
    {
        std::wcout << L"Likely running on Win7 or older OS, and KB2533623 is not installed" << std::endl;
    }

    if (kb2533623Exists)
    {
        hModule = LoadLibraryEx(L"UCRTBASE.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }
    else
    {
        hModule = LoadLibrary(L"UCRTBASE.dll");
    }

    if (hModule != nullptr)
    {
        std::wcout << L"UCRT is available - Either running on Win10+ or KB2999226 is installed" << std::endl;
    }
    else
    {
        std::wcout << L"UCRT is not available - Likely running on OS older than Win10 and KB2999226 is not installed" << std::endl;
    }

    FreeLibrary(hModule);

    return 0;
}