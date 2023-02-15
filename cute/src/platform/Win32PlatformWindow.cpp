#include "Win32PlatformWindow.hpp"

#include <cassert>
#include <string>

#include "Win32Platform.hpp"

LRESULT Win32PlatformWindow::dispatchWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	auto *instance = static_cast<Win32PlatformWindow *>(GetPropW(window, THIS_PROP_NAME));

	if (!instance)
		return DefWindowProcW(window, message, wParam, lParam);

	return instance->windowProc(window, message, wParam, lParam);
}

Win32PlatformWindow::Win32PlatformWindow() : _handle(createWindow()) {
	SetPropW(_handle, THIS_PROP_NAME, this);
}

Win32PlatformWindow::~Win32PlatformWindow() {
	DestroyWindow(_handle);
}

void Win32PlatformWindow::setClientSize(int width, int height) {
	RECT windowRect = {0, 0, width, height};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	int adjustedWidth = windowRect.right - windowRect.left;
	int adjustedHeight = windowRect.bottom - windowRect.top;

	SetWindowPos(
			_handle,
			nullptr, // insertAfter
			0, 0, // position
			adjustedWidth, adjustedHeight,
			SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOZORDER
	);
}

static std::wstring convertUtf8ToUtf16(const std::string &src) {
	std::wstring dst;

	int charCount = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, nullptr, 0);

	if (charCount < 1)
		return dst;

	dst.resize(charCount);

	if (MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, dst.data(), charCount) != charCount)
		return {};

	return dst;
}

void Win32PlatformWindow::setTitle(const std::string &title) {
	SetWindowTextW(_handle, convertUtf8ToUtf16(title).c_str());
}

void Win32PlatformWindow::show() {
	ShowWindow(_handle, SW_SHOWDEFAULT);
}

HWND Win32PlatformWindow::createWindow() {
	return CreateWindowExW(
			WS_EX_NOREDIRECTIONBITMAP,
			Win32Platform::WINDOW_CLASS_NAME,
			nullptr,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, // position
			CW_USEDEFAULT, CW_USEDEFAULT, // size
			nullptr, // parent
			nullptr, // menu
			GetModuleHandleW(nullptr),
			nullptr // param
	);
}

LRESULT Win32PlatformWindow::windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProcW(window, message, wParam, lParam);
	}

	return 0;
}