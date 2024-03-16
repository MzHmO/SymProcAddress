#include <windows.h>
#include <Dbghelp.h>
#include <iostream>
#include <map>
#include <string>

#pragma comment(lib, "Dbghelp.lib") 

FARPROC SymProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	static BOOL gathered = FALSE;
	static std::map<std::string, FARPROC> funcs;

	if (!gathered)
	{
		HANDLE hProc = GetCurrentProcess();
		SymInitialize(hProc, NULL, TRUE);
		auto EnumSymbolsCallback = [](PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext) -> BOOL {
			if (pSymInfo->Flags & 0x00000200) {
				funcs[std::string(pSymInfo->Name)] = reinterpret_cast<FARPROC>(pSymInfo->Address);
			}
			return TRUE;
			};
#ifdef _WIN64
		if (!SymEnumSymbols(hProc, (DWORD64)hModule, NULL, EnumSymbolsCallback, NULL)) {
			SymCleanup(hProc);
			return NULL;
		}
#else
		if (!SymEnumSymbols(hProc, (DWORD)hModule, NULL, EnumSymbolsCallback, NULL)) {
			SymCleanup(hProc);
			return NULL;
		}
#endif
	}

	auto it = funcs.find(std::string(lpProcName));
	if (it != funcs.end()) {
		return it->second;
	}
	else {
		SetLastError(127); // ERROR_PROC_NOT_FOUND
		return NULL;
	}
}


// Example Usage
typedef int (WINAPI* MessageBoxWFunc)(
	HWND    hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT    uType
	);

int main()
{
	HMODULE hModule = NULL;

	//hModule = GetModuleHandle("user32.dll");
	hModule = LoadLibraryA("user32.dll"); 
	MessageBoxWFunc MessageBoxWPtr = (MessageBoxWFunc)(SymProcAddress(hModule, "MessageBoxW"));
	MessageBoxWPtr(NULL, L"Lol who said GetProcAddress() xD", L"Hi from MzHmO", MB_OK);

	return 0;
}