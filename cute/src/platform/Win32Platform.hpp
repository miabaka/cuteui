#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include "cute/ui/platform/Platform.hpp"

class Win32Platform : public Platform {
public:
	static constexpr auto WINDOW_CLASS_NAME = L"CuteWindow";

	Win32Platform();

	~Win32Platform() override;

	std::unique_ptr<PlatformWindow> createWindow() override;

private:
	WNDCLASSEXW _windowClass;

	static WNDCLASSEXW registerWindowClass();

	void unregisterWindowClass() const;
};