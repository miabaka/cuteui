#include "Win32Window.hpp"

#include <string>
#include <dwmapi.h>
#include "Win32Platform.hpp"

using namespace cutegfx;

LRESULT Win32Window::dispatchWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	Win32Window *instance;

	if (message == WM_NCCREATE) {
		auto *createStruct = reinterpret_cast<CREATESTRUCTW *>(lParam);

		instance = static_cast<Win32Window *>(createStruct->lpCreateParams);
		instance->_handle = window;

		SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
	} else {
		instance = reinterpret_cast<Win32Window *>(GetWindowLongPtrW(window, GWLP_USERDATA));
	}

	if (!instance)
		return DefWindowProcW(window, message, wParam, lParam);

	return instance->windowProc(message, wParam, lParam);
}

Win32Window::Win32Window() {
	createWindow();
}

Win32Window::~Win32Window() {
	SetWindowLongPtrW(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
	DestroyWindow(_handle);
}

HWND Win32Window::getHandle() const {
	return _handle;
}

static glm::ivec2 rectToSize(RECT rect) {
	return {rect.right - rect.left, rect.bottom - rect.top};
}

glm::ivec2 Win32Window::getClientSize() const {
	return _clientSize;
}

void Win32Window::setClientSize(glm::ivec2 size) {
	_clientSize = size;

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

void Win32Window::setTitle(const std::string &title) {
	SetWindowTextW(_handle, convertUtf8ToUtf16(title).c_str());
}

Window::BackdropType Win32Window::getBackdropType() const {
	return _backdropType;
}

bool Win32Window::setBackdropType(BackdropType backdropType) {
	if (backdropType == _backdropType)
		return true;

	if (!setMicaEffectEnabled(backdropType == BackdropType::Mica))
		return false;

	_backdropType = backdropType;

	return true;
}

bool Win32Window::isVisible() const {
	return IsWindowVisible(_handle);
}

void Win32Window::setVisible(bool visible) {
	ShowWindow(_handle, visible ? SW_SHOWDEFAULT : SW_HIDE);
}

void Win32Window::createWindow() {
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

void Win32Window::trySetResizeState(ResizeState state) {
	if (state == _resizeState)
		return;

	switch (_resizeState) {
		case ResizeState::None:
			if (state != ResizeState::EnteredLoop)
				return;

			break;

		case ResizeState::EnteredLoop:
			if (state == ResizeState::Resizing)
				sResizeBegin.emit();

			break;

		case ResizeState::Resizing:
			if (state != ResizeState::None)
				return;

			sResizeEnd.emit();
	}

	_resizeState = state;
}

LRESULT Win32Window::windowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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
			sVisibilityChange.emit(wParam);
			return 0;

		case WM_SYSCOMMAND:
			if (wParam == SC_KEYMENU)
				return 0;

			break;

		case WM_ERASEBKGND:
			return true;

		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE) {
				sFocus.emit();
				return 0;
			}

		case WM_WINDOWPOSCHANGED: {
			auto *windowPos = reinterpret_cast<WINDOWPOS *>(lParam);

			if (!windowPos || windowPos->flags & SWP_NOSIZE)
				break;

			RECT clientRect;
			GetClientRect(_handle, &clientRect);

			_clientSize = {clientRect.right - clientRect.left, clientRect.bottom - clientRect.top};

			trySetResizeState(ResizeState::Resizing);

			return 0;
		}

		case WM_ENTERSIZEMOVE:
			trySetResizeState(ResizeState::EnteredLoop);
			return 0;

		case WM_EXITSIZEMOVE:
			trySetResizeState(ResizeState::None);
			return 0;

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

bool Win32Window::setMicaEffectEnabled(bool enabled) {
	if (setMicaEffectEnabled22523(_handle, enabled))
		return true;

	if (setMicaEffectEnabledPre22523(_handle, enabled))
		return true;

	return false;
}