#include "cuteui/system/WindowManager.hpp"

#include "cuteui/Application.hpp"

void WindowManager::registerVisibleWindow(const ctl::RcPtr<Window> &window) {
	std::scoped_lock lock(_visibleWindowsMutex);
	_visibleWindows.emplace(window);
	_visibleWindowsChanged = true;
}

void WindowManager::unregisterVisibleWindow(const ctl::RcPtr<Window> &window) {
	std::scoped_lock lock(_visibleWindowsMutex);

	_visibleWindows.erase(window);

	if (_lastActiveWindow == window)
		_lastActiveWindow = nullptr;

	_visibleWindowsChanged = true;
}

ctl::RcPtr<Window> WindowManager::getLastActiveWindow() {
	std::scoped_lock lock(_lastActiveWindowMutex);
	return _lastActiveWindow;
}

void WindowManager::setLastActiveWindow(const ctl::RcPtr<Window> &window) {
	std::scoped_lock lock(_lastActiveWindowMutex);
	_lastActiveWindow = window;
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

void WindowManager::setWaitSync(bool waitSync) {
	_waitSync = waitSync;
}

void WindowManager::renderMain() {
	cutegfx::Renderer &renderer = Application::getInstance()
			.getPlatform()
			.getRenderer();

	std::vector<ctl::RcPtr<Window>> windows;

	while (true) {
		if (_visibleWindowsChanged) {
			std::scoped_lock lock(_visibleWindowsMutex);
			_visibleWindowsChanged = false;
			windows.assign(_visibleWindows.begin(), _visibleWindows.end());
		}

		renderer.beginFrame();

		if (windows.empty()) {
			sUpdate(UpdateType::EventLoopStopRequest);
			break;
		}

		sUpdate(UpdateType::Update);

		renderer.render();

		ctl::RcPtr<Window> lastActiveWindow = getLastActiveWindow();

		bool waitSync = _waitSync;

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