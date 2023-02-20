#include "cuteui/system/WindowManager.hpp"

void WindowManager::registerWindow(Window *window) {
	_windows.insert(window);
}

void WindowManager::unregisterWindow(Window *window) {
	_windows.erase(window);
}

void WindowManager::registerVisibleWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_visibleWindowsMutex);
	_visibleWindows.insert(window);
	_visibleWindowsChanged = true;
}

void WindowManager::unregisterVisibleWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_visibleWindowsMutex);

	_visibleWindows.erase(window);

	if (!_visibleWindows.empty())
		return;

	_visibleWindowsChanged = true;
}

void WindowManager::updateWindows() {
	for (auto &window: _windows)
		window->update();
}

void WindowManager::startRenderThread() {
	_renderThread = std::thread(&WindowManager::renderMain, this);
}

void WindowManager::joinRenderThread() {
	_renderThread.join();
}

void WindowManager::renderMain() {
	std::vector<std::weak_ptr<Window>> windows;

	while (true) {
		if (_visibleWindowsChanged) {
			std::scoped_lock lock(_visibleWindowsMutex);

			_visibleWindowsChanged = false;

			if (_visibleWindows.empty()) {
				sUpdate.emit(UpdateType::EventLoopStopRequest);
				break;
			}

			windows.assign(_visibleWindows.begin(), _visibleWindows.end());
		}

		sUpdate.emit(UpdateType::Update);

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