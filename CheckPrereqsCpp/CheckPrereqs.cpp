#include <iostream>
#include <windows.h>

typedef std::shared_ptr < std::decay_t<decltype(*std::declval<HMODULE>())>> SafeModulePointer;

int main()
{
	// Initially, kernel32.dll doesn't need any specific LoadLibraryEx flags. 
	// Later, we will set LOAD_LIBRARY_SEARCH_SYSTEM32 for loading ucrtbase.dll if it is supported.
	DWORD dwLoadLibraryFlags = 0; 

	SafeModulePointer hModule(
		LoadLibraryExW(L"kernel32.dll", nullptr, dwLoadLibraryFlags), 
		FreeLibrary);

    if (hModule)
    {
		// AddDllDirectory and LOAD_LIBRARY_SEARCH_SYSTEM32 flag for LoadLibraryEx were introduced by KB2533623. 
		// If this function is present in kernel32.dll, then KB2533623 or equivalent support is present
        auto hFarProc = GetProcAddress(hModule.get(), "AddDllDirectory");
		if (hFarProc != nullptr)
		{
			std::wcout << L"Either running on Win8+, or KB2533623 is installed" << std::endl;
			dwLoadLibraryFlags = LOAD_LIBRARY_SEARCH_SYSTEM32;
		}
		else
		{
			std::wcout << L"Likely running on Win7 or older OS, and KB2533623 is not installed" << std::endl;
		}
    }

	// KB2999226 provides the Universal CRT libraries. 
	// A key DLL in UCRT is ucrtbase.dll, which is present in %windir%\system32. 
	// If UCRT and all its dependencies are present in the OS, we should be able to load it without any problems. 
	// This tool is compiled with static-UCRT libraries, and doesn't require UCRT DLL's to run. 
	SafeModulePointer hUCRTModule(
		LoadLibraryEx(L"UCRTBASE.dll", nullptr, dwLoadLibraryFlags),
		FreeLibrary);
    if (hUCRTModule)
    {
        std::wcout << L"UCRT is available - Either running on Win10+ or KB2999226 is installed" << std::endl;
    }
    else
    {
        std::wcout << L"UCRT is not available - Likely running on OS older than Win10 and KB2999226 is not installed" << std::endl;
    }

	return 0;
}
