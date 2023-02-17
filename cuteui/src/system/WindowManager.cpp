#include "cuteui/system/WindowManager.hpp"

WindowManager::~WindowManager() {
	stopRenderThread();
}

bool WindowManager::hasWindows() const {
	return _hasWindows;
}

void WindowManager::registerWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_windowListMutex);

	if (!_renderThreadIsRunning)
		startRenderThread();

	_windows.insert(window);

	_hasWindows = true;
	_windowListChanged = true;
}

void WindowManager::unregisterWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_windowListMutex);

	_windows.erase(window);

	_hasWindows = !_windows.empty();
	_windowListChanged = true;

	if (!hasWindows())
		stopRenderThread();
}

void WindowManager::startRenderThread() {
	_renderThreadIsRunning = true;
	_renderThread = std::thread(&WindowManager::renderMain, this);
}

void WindowManager::stopRenderThread() {
	if (!_renderThreadIsRunning)
		return;

	_renderThreadIsRunning = false;
	_renderThread.join();
}

void WindowManager::renderMain() {
	std::vector<std::shared_ptr<Window>> windows;

	while (_renderThreadIsRunning) {
		if (_windowListChanged) {
			std::scoped_lock lock(_windowListMutex);
			windows.assign(_windows.begin(), _windows.end());
			_windowListChanged = false;
		}

		bool waitSync = true;

		for (auto &window: windows) {
			window->render(waitSync);
			waitSync = false;
		}
	}
}