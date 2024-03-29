#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include "cutegfx/Platform.hpp"
#include "../d3d11/Direct3D11Device.hpp"

#include "Win32HelperWindow.hpp"

namespace cutegfx {

class Win32Platform : public Platform {
public:
	static constexpr auto WINDOW_CLASS_NAME = L"CuteWindow";
	static constexpr auto HELPER_WINDOW_CLASS_NAME = L"CuteHelper";

	Win32Platform();

	~Win32Platform() override;

	ctl::RcPtr<Window> createWindow() override;

	Renderer &getRenderer() override;

	TextureLoader &getTextureLoader() override;

	int runEventLoop(std::function<void()> tickHandler) override;

	void executeTickHandlerIndirect(Platform::TickType tickType) override;

private:
	WNDCLASSEXW _windowClass;
	WNDCLASSEXW _helperWindowClass;
	ctl::RcPtr<Renderer> _renderer;
	ctl::RcPtr<TextureLoader> _textureLoader;
	Win32HelperWindow _helperWindow;

	static WNDCLASSEXW registerWindowClass();

	static WNDCLASSEXW registerHelperWindowClass();

	void unregisterWindowClasses() const;
};

} // namespace cutegfx