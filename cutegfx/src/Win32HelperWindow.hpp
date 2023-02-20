#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <functional>
#include <thread>

#include <windows.h>

#include "cutegfx/Platform.hpp"

class Win32HelperWindow {
public:
	static LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	Win32HelperWindow();

	~Win32HelperWindow();

	void setTickHandler(std::function<void(void)> handler);

	void executeTickHandler();

	void executeTickHandlerIndirect(Platform::TickType tickType);

private:
	static constexpr auto THIS_PROP_NAME = L"CuteHelper";

	HWND _handle;
	std::function<void()> _tickHandler;

	static HWND createWindow();
};