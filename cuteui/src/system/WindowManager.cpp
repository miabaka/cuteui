#include <utility>

#include "cuteui/system/WindowManager.hpp"

bool WindowManager::hasVisibleWindows() const {
	return _hasVisibleWindows;
}

void WindowManager::registerWindow(Window *window) {
	_windows.insert(window);
}

void WindowManager::unregisterWindow(Window *window) {
	_windows.erase(window);
}

void WindowManager::registerVisibleWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_visibleWindowsMutex);

	_visibleWindows.insert(window);

	_hasVisibleWindows = true;
	_visibleWindowsChanged = true;
}

void WindowManager::unregisterVisibleWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_visibleWindowsMutex);

	_visibleWindows.erase(window);

	if (!_visibleWindows.empty())
		return;

	_hasVisibleWindows = false;
	_visibleWindowsChanged = true;

	_lastVisibleWindowClosedHandler();
}

void WindowManager::updateWindows() {
	for (auto &window: _windows)
		window->update();
}

void WindowManager::startRenderThread() {
	_renderThreadRunning = true;
	_renderThread = std::thread(&WindowManager::renderMain, this);
}

void WindowManager::stopRenderThread() {
	_renderThreadRunning = false;
	_renderThread.join();
}

void WindowManager::renderMain() {
	std::vector<std::weak_ptr<Window>> windows;

	while (_renderThreadRunning) {
		_updateHandler();

		if (_visibleWindowsChanged) {
			std::scoped_lock lock(_visibleWindowsMutex);
			_visibleWindowsChanged = false;
			windows.assign(_visibleWindows.begin(), _visibleWindows.end());
		}

		bool waitSync = true;

		for (auto &weakWindow: windows) {
			auto window = weakWindow.lock();

			if (!window)
				continue;

			window->render(waitSync);

			waitSync = false;
		}
	}
}