#include "cuteui/system/WindowManager.hpp"

bool WindowManager::hasWindows() const {
	return !_windows.empty();
}

void WindowManager::registerWindow(std::shared_ptr<Window> window) {
	_windows.insert(window);
}

void WindowManager::unregisterWindow(std::shared_ptr<Window> window) {
	_windows.erase(window);
}

void WindowManager::tickWindows() {
	bool waitSync = true;

	for (auto &window: _windows) {
		window->render(waitSync);
		waitSync = false;
	}
}