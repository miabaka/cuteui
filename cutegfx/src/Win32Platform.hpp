#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include "cutegfx/Platform.hpp"
#include "Direct3D11Renderer.hpp"

class Win32Platform : public Platform {
public:
	static constexpr auto WINDOW_CLASS_NAME = L"CuteWindow";
	static constexpr auto HELPER_WINDOW_CLASS_NAME = L"CuteHelper";

	Win32Platform();

	~Win32Platform() override;

	std::shared_ptr<PlatformWindow> createWindow() override;

	Renderer &getRenderer() override;

	void runEventLoop(std::function<void()> tickHandler) override;

private:
	WNDCLASSEXW _windowClass;
	WNDCLASSEXW _helperWindowClass;
	std::shared_ptr<Direct3D11Renderer> _renderer;

	static WNDCLASSEXW registerWindowClass();

	static WNDCLASSEXW registerHelperWindowClass();

	void unregisterWindowClasses() const;
};