#include "Win32HelperWindow.hpp"

#include <stdexcept>
#include <string>
#include <utility>

#include "Win32Platform.hpp"

LRESULT Win32HelperWindow::windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	auto *instance = static_cast<Win32HelperWindow *>(GetPropW(window, THIS_PROP_NAME));

	switch (message) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(window, &ps);
			EndPaint(window, &ps);
			return 0;
		}

		case WM_USER:
			if (instance) {
				instance->_tickHandler();
				return true;
			}

		default:
			break;
	}

	return DefWindowProcW(window, message, wParam, lParam);
}

Win32HelperWindow::Win32HelperWindow() : _handle(createWindow()) {
	SetPropW(_handle, THIS_PROP_NAME, this);
}

Win32HelperWindow::~Win32HelperWindow() {
	RemovePropW(_handle, THIS_PROP_NAME);
	DestroyWindow(_handle);
}

void Win32HelperWindow::setTickHandler(std::function<void()> handler) {
	_tickHandler = std::move(handler);
}

void Win32HelperWindow::executeTickHandler() {
	if (!_tickHandler)
		return;

	_tickHandler();
}

void Win32HelperWindow::executeTickHandlerIndirect() {
	SendMessageW(_handle, WM_USER, 0, 0);
}

HWND Win32HelperWindow::createWindow() {
	HWND handle = CreateWindowExW(
			0, // exStyle
			Win32Platform::HELPER_WINDOW_CLASS_NAME,
			nullptr,
			0, // style
			0, 0, // position
			0, 0, // size
			HWND_MESSAGE, // parent
			nullptr, // menu
			GetModuleHandleW(nullptr),
			nullptr // param
	);

	if (!handle)
		throw std::runtime_error("CreateWindowExW failed: " + std::to_string(GetLastError()));

	return handle;
}
