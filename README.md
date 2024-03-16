# SymProcAddress
Zero EAT touch way to retrieve function addresses

![изображение](https://github.com/MzHmO/SymProcAddress/assets/92790655/498974fc-48c8-4da6-b727-77f969c7ed3c)


## TL;DR

Just check the example usage, this is similar to the standard GetProcAddress function:
```cpp
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
```

You only need to include the function code in your project and you will be able to use my method of function address resolution

## How It Works
I discovered that we can use DbgHelp to list all symbols in a PE image. The program lists all symbols based on the base address (`hModule`). So, when you call `SymFuncAddress()` for the first time, the program will create a dictionary with "function name - function address" key-value pairs, after which you can get the addresses of any functions you are interested in from the Dll.
