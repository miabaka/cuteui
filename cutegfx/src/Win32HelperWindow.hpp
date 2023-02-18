#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <functional>
#include <thread>

#include <windows.h>

class Win32HelperWindow {
public:
	static LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	Win32HelperWindow(std::function<void(void)> tickHandler);

	~Win32HelperWindow();

	void executeTickHandler();

private:
	static constexpr auto THIS_PROP_NAME = L"CuteHelper";

	HWND _handle;
	std::function<void()> _tickHandler;
	std::thread _tickerThread;

	static HWND createWindow();

	void tickerMain();
};