#include <utility>

#include "cuteui/system/WindowManager.hpp"

bool WindowManager::hasVisibleWindows() const {
	return !_visibleWindows.empty();
}

void WindowManager::registerWindow(Window *window) {
	_windows.insert(window);
}

void WindowManager::unregisterWindow(Window *window) {
	_windows.erase(window);
}

void WindowManager::registerVisibleWindow(std::shared_ptr<Window> window) {
	_visibleWindows.insert(window);
}

void WindowManager::unregisterVisibleWindow(std::shared_ptr<Window> window) {
	_visibleWindows.erase(window);

	if (!hasVisibleWindows())
		_lastVisibleWindowClosedHandler();
}

void WindowManager::updateWindows() {
	for (auto &window: _windows)
		window->update();
}

void WindowManager::renderWindows() {
	bool waitSync = true;

	for (auto &window: _visibleWindows) {
		window->render(waitSync);
		waitSync = false;
	}
}

void WindowManager::startRenderThread() {
	_renderThread = std::thread(&WindowManager::renderMain, this);
}

void WindowManager::stopRenderThread() {
	_renderThreadRunning = false;
	_renderThread.join();
}

void WindowManager::renderMain() {
	_renderThreadRunning = true;

	while (_renderThreadRunning) {
		_updateHandler();
		renderWindows();
	}
}