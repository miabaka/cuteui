#include "Win32PlatformWindow.hpp"

#include <string>
#include <dwmapi.h>
#include "Win32Platform.hpp"

LRESULT Win32PlatformWindow::dispatchWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	Win32PlatformWindow *instance;

	if (message == WM_NCCREATE) {
		auto *createStruct = reinterpret_cast<CREATESTRUCT *>(lParam);

		instance = static_cast<Win32PlatformWindow *>(createStruct->lpCreateParams);
		instance->_handle = window;

		SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
	} else {
		instance = reinterpret_cast<Win32PlatformWindow *>(GetWindowLongPtrW(window, GWLP_USERDATA));
	}

	if (!instance)
		return DefWindowProcW(window, message, wParam, lParam);

	return instance->windowProc(message, wParam, lParam);
}

Win32PlatformWindow::Win32PlatformWindow() {
	createWindow();
}

Win32PlatformWindow::~Win32PlatformWindow() {
	SetWindowLongPtrW(_handle, GWLP_USERDATA, 0);
	DestroyWindow(_handle);
}

HWND Win32PlatformWindow::getHandle() const {
	return _handle;
}

static glm::ivec2 rectToSize(RECT rect) {
	return {rect.right - rect.left, rect.bottom - rect.top};
}

glm::ivec2 Win32PlatformWindow::getClientSize() const {
	RECT clientRect;
	GetClientRect(_handle, &clientRect);
	return rectToSize(clientRect);
}

void Win32PlatformWindow::setClientSize(glm::ivec2 size) {
	RECT windowRect = {0, 0, size.x, size.y};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	glm::ivec2 adjustedSize = rectToSize(windowRect);

	SetWindowPos(
			_handle,
			nullptr, // insertAfter
			0, 0, // position
			adjustedSize.x, adjustedSize.y,
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

PlatformWindow::BackdropType Win32PlatformWindow::getBackdropType() const {
	return _backdropType;
}

bool Win32PlatformWindow::setBackdropType(BackdropType backdropType) {
	if (backdropType == _backdropType)
		return true;

	if (!setMicaEffectEnabled(backdropType == BackdropType::Mica))
		return false;

	_backdropType = backdropType;

	return true;
}

bool Win32PlatformWindow::isVisible() const {
	return IsWindowVisible(_handle);
}

void Win32PlatformWindow::setVisible(bool visible) {
	ShowWindow(_handle, visible ? SW_SHOWDEFAULT : SW_HIDE);
}

void Win32PlatformWindow::createWindow() {
	CreateWindowExW(
			WS_EX_NOREDIRECTIONBITMAP,
			Win32Platform::WINDOW_CLASS_NAME,
			nullptr,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, // position
			CW_USEDEFAULT, CW_USEDEFAULT, // size
			nullptr, // parent
			nullptr, // menu
			GetModuleHandleW(nullptr),
			this // param
	);
}

LRESULT Win32PlatformWindow::windowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(_handle, &ps);
			EndPaint(_handle, &ps);
			return 0;
		}

		case WM_CLOSE:
			setVisible(false);
			return 0;

		case WM_SHOWWINDOW:
			visibilityChanged.emit(wParam);
			return 0;

		case WM_SYSCOMMAND:
			if (wParam == SC_KEYMENU)
				return 0;

			break;

		case WM_ERASEBKGND:
			return true;

		default:
			break;
	}

	return DefWindowProcW(_handle, message, wParam, lParam);
}

// Windows 11 build 22523+ method
static bool setMicaEffectEnabled22523(HWND window, bool enabled) {
	const auto DWMWA_SYSTEMBACKDROP_TYPE_ = 38;

	enum {
		DwmBackdropType_Auto = 0,
		DwmBackdropType_MainWindow = 2
	} backdropType = enabled ? DwmBackdropType_MainWindow : DwmBackdropType_Auto;

	HRESULT result = DwmSetWindowAttribute(window, DWMWA_SYSTEMBACKDROP_TYPE_, &backdropType, sizeof(backdropType));

	return SUCCEEDED(result);
}

// Windows 11 pre 22523 build method
static bool setMicaEffectEnabledPre22523(HWND window, bool enabled) {
	const auto DWMWA_MICA_EFFECT_ = 1029;

	BOOL value = enabled;
	HRESULT result = DwmSetWindowAttribute(window, DWMWA_MICA_EFFECT_, &value, sizeof(value));

	return SUCCEEDED(result);
}

bool Win32PlatformWindow::setMicaEffectEnabled(bool enabled) {
	if (setMicaEffectEnabled22523(_handle, enabled))
		return true;

	if (setMicaEffectEnabledPre22523(_handle, enabled))
		return true;

	return false;
}