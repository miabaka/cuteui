#include "Win32HelperWindow.hpp"

#include <stdexcept>
#include <string>
#include <utility>

#include "Win32Platform.hpp"

using namespace cutegfx;

LRESULT Win32HelperWindow::windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	Win32HelperWindow *instance;

	if (message == WM_NCCREATE) {
		auto *createStruct = reinterpret_cast<CREATESTRUCT *>(lParam);

		instance = static_cast<Win32HelperWindow *>(createStruct->lpCreateParams);
		instance->_handle = window;

		SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
	} else {
		instance = reinterpret_cast<Win32HelperWindow *>(GetWindowLongPtrW(window, GWLP_USERDATA));
	}

	if (!instance)
		return DefWindowProcW(window, message, wParam, lParam);

	switch (message) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(window, &ps);
			EndPaint(window, &ps);
			return 0;
		}

		case WM_USER:
			if (static_cast<Platform::TickType>(wParam) == Platform::TickType::Update)
				instance->executeTickHandler();
			else
				PostQuitMessage(0);

			return true;

		default:
			break;
	}

	return DefWindowProcW(window, message, wParam, lParam);
}

Win32HelperWindow::Win32HelperWindow() {
	createWindow();
}

Win32HelperWindow::~Win32HelperWindow() {
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

void Win32HelperWindow::executeTickHandlerIndirect(Platform::TickType tickType) {
	SendMessageW(_handle, WM_USER, static_cast<WPARAM>(tickType), 0);
}

void Win32HelperWindow::createWindow() {
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
			this // param
	);

	if (!handle)
		throw std::runtime_error("CreateWindowExW failed: " + std::to_string(GetLastError()));
}
