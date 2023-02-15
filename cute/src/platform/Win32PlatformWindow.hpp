#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include "cute/ui/platform/PlatformWindow.hpp"

class Win32PlatformWindow : public PlatformWindow {
public:
	static constexpr auto THIS_PROP_NAME = L"CuteWindow";

	static LRESULT CALLBACK dispatchWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	Win32PlatformWindow();

	~Win32PlatformWindow() override;

	void setClientSize(int width, int height) override;

	void setTitle(const std::string &title) override;

	void show() override;

private:
	HWND _handle;

	static HWND createWindow();

	LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
};