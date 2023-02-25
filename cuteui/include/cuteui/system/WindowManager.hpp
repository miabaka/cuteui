#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

#include <cutegfx/Platform.hpp>
#include <cuteutil/SingleSignal.hpp>

#include "../Window.hpp"

class WindowManager {
public:
	using UpdateType = cutegfx::Platform::TickType;

	void registerVisibleWindow(std::shared_ptr<Window> window);

	void unregisterVisibleWindow(std::shared_ptr<Window> window);

	std::shared_ptr<Window> getLastActiveWindow();

	void setLastActiveWindow(std::shared_ptr<Window> window);

	void updateWindows();

	void startRenderThread();

	void joinRenderThread();

	void setWaitSync(bool waitSync = true);

	cuteutil::SingleSignal<UpdateType> sUpdate;

private:
	std::unordered_set<std::shared_ptr<Window>> _visibleWindows;
	std::shared_ptr<Window> _lastActiveWindow;
	std::thread _renderThread;
	std::mutex _visibleWindowsMutex;
	std::mutex _lastActiveWindowMutex;
	std::atomic<bool> _visibleWindowsChanged = false;
	std::atomic<bool> _waitSync = true;

	void renderMain();
};