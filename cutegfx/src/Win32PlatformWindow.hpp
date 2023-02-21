#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include "cutegfx/PlatformWindow.hpp"

namespace cutegfx {

class Win32PlatformWindow : public PlatformWindow {
public:
	static LRESULT CALLBACK dispatchWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	Win32PlatformWindow();

	~Win32PlatformWindow() override;

	HWND getHandle() const;

	glm::ivec2 getClientSize() const override;

	void setClientSize(glm::ivec2 size) override;

	void setTitle(const std::string &title) override;

	BackdropType getBackdropType() const override;

	bool setBackdropType(BackdropType backdropType) override;

	bool isVisible() const override;

	void setVisible(bool visible) override;

private:
	HWND _handle{};
	BackdropType _backdropType = BackdropType::Default;

	void createWindow();

	LRESULT CALLBACK windowProc(UINT message, WPARAM wParam, LPARAM lParam);

	bool setMicaEffectEnabled(bool enabled = true);
};

} // namespace cutegfx