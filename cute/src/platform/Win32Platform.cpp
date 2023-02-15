#include "Win32Platform.hpp"

#include <stdexcept>
#include <string>

#include "Win32PlatformWindow.hpp"

Win32Platform::Win32Platform()
		: _windowClass(registerWindowClass()) {}

Win32Platform::~Win32Platform() {
	unregisterWindowClass();
}

std::unique_ptr<PlatformWindow> Win32Platform::createWindow() {
	return std::make_unique<Win32PlatformWindow>();
}

WNDCLASSEXW Win32Platform::registerWindowClass() {
	WNDCLASSEXW windowClass{};

	windowClass.cbSize = sizeof(windowClass);
	windowClass.lpfnWndProc = Win32PlatformWindow::dispatchWindowProc;
	windowClass.hInstance = GetModuleHandleW(nullptr);
	windowClass.hCursor = LoadCursorA(nullptr, IDC_ARROW);
	windowClass.lpszClassName = WINDOW_CLASS_NAME;

	if (RegisterClassExW(&windowClass) == 0)
		throw std::runtime_error("RegisterClassExW failed: " + std::to_string(GetLastError()));

	return windowClass;
}

void Win32Platform::unregisterWindowClass() const {
	UnregisterClassW(_windowClass.lpszClassName, _windowClass.hInstance);
}
