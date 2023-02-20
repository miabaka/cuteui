#include "Win32Platform.hpp"

#include <stdexcept>
#include <string>

#include "Win32PlatformWindow.hpp"

Win32Platform::Win32Platform()
		: _windowClass(registerWindowClass()),
		  _helperWindowClass(registerHelperWindowClass()),
		  _renderer(Direct3D11Renderer::create()) {}

Win32Platform::~Win32Platform() {
	unregisterWindowClasses();
}

std::shared_ptr<PlatformWindow> Win32Platform::createWindow() {
	return std::make_shared<Win32PlatformWindow>();
}

Renderer &Win32Platform::getRenderer() {
	return *_renderer;
}

void Win32Platform::runEventLoop(std::function<void()> tickHandler) {
	_helperWindow.setTickHandler(tickHandler);

	MSG msg;

	while (GetMessageW(&msg, nullptr, 0, 0) == TRUE) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	// Dispatch remaining messages
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void Win32Platform::executeTickHandlerIndirect(Platform::TickType tickType) {
	_helperWindow.executeTickHandlerIndirect(tickType);
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

WNDCLASSEXW Win32Platform::registerHelperWindowClass() {
	WNDCLASSEXW windowClass{};

	windowClass.cbSize = sizeof(windowClass);
	windowClass.lpfnWndProc = Win32HelperWindow::windowProc;
	windowClass.hInstance = GetModuleHandleW(nullptr);
	windowClass.lpszClassName = HELPER_WINDOW_CLASS_NAME;

	if (RegisterClassExW(&windowClass) == 0)
		throw std::runtime_error("RegisterClassExW failed: " + std::to_string(GetLastError()));

	return windowClass;
}

void Win32Platform::unregisterWindowClasses() const {
	UnregisterClassW(_windowClass.lpszClassName, _windowClass.hInstance);
	UnregisterClassW(_helperWindowClass.lpszClassName, _helperWindowClass.hInstance);
}