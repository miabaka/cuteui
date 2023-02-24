#include "cuteui/system/WindowManager.hpp"

#include "cuteui/Application.hpp"

void WindowManager::registerVisibleWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_visibleWindowsMutex);
	_visibleWindows.insert(window);
	_visibleWindowsChanged = true;
}

void WindowManager::unregisterVisibleWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_visibleWindowsMutex);

	_visibleWindows.erase(window);

	if (_lastActiveWindow == window)
		_lastActiveWindow = nullptr;

	_visibleWindowsChanged = true;
}

std::shared_ptr<Window> WindowManager::getLastActiveWindow() {
	std::scoped_lock lock(_lastActiveWindowMutex);
	return _lastActiveWindow;
}

void WindowManager::setLastActiveWindow(std::shared_ptr<Window> window) {
	std::scoped_lock lock(_lastActiveWindowMutex);
	_lastActiveWindow = std::move(window);
}

void WindowManager::updateWindows() {
	cutegfx::Renderer &renderer = Application::getInstance()
			.getPlatform()
			.getRenderer();

	for (auto &window: _visibleWindows) {
		window->updateLayout();
		window->draw(renderer);
	}
}

void WindowManager::startRenderThread() {
	_renderThread = std::thread(&WindowManager::renderMain, this);
}

void WindowManager::joinRenderThread() {
	_renderThread.join();
}

void WindowManager::renderMain() {
	cutegfx::Renderer &renderer = Application::getInstance()
			.getPlatform()
			.getRenderer();

	std::vector<std::shared_ptr<Window>> windows;

	while (true) {
		if (_visibleWindowsChanged) {
			std::scoped_lock lock(_visibleWindowsMutex);
			_visibleWindowsChanged = false;
			windows.assign(_visibleWindows.begin(), _visibleWindows.end());
		}

		if (windows.empty()) {
			sUpdate.emit(UpdateType::EventLoopStopRequest);
			break;
		}

		sUpdate.emit(UpdateType::Update);

 		renderer.render();

		std::shared_ptr<Window> lastActiveWindow = getLastActiveWindow();

		bool waitSync = true;

		if (lastActiveWindow) {
			lastActiveWindow->present(waitSync);
			waitSync = false;
		}

		for (auto &window: windows) {
			if (window == lastActiveWindow)
				continue;

			window->present(waitSync && !lastActiveWindow);
			waitSync = false;
		}
	}
}