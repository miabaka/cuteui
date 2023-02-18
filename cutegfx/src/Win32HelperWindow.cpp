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
				instance->executeTickHandler();
				return true;
			}

		default:
			break;
	}

	return DefWindowProcW(window, message, wParam, lParam);
}

Win32HelperWindow::Win32HelperWindow(std::function<void()> tickHandler)
		: _handle(createWindow()),
		  _tickHandler(std::move(tickHandler)),
		  _tickerThread(&Win32HelperWindow::tickerMain, this) {
	SetPropW(_handle, THIS_PROP_NAME, this);
}

Win32HelperWindow::~Win32HelperWindow() {
	RemovePropW(_handle, THIS_PROP_NAME);
	DestroyWindow(_handle);
	_tickerThread.join();
}

void Win32HelperWindow::executeTickHandler() {
	_tickHandler();
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

void Win32HelperWindow::tickerMain() {
	while (true) {
		if (!SendMessageW(_handle, WM_USER, 0, 0))
			break;
	}
}