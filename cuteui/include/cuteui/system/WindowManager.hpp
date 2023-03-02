#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

#include <ctl/memory.hpp>
#include <ctl/Signal.hpp>
#include <cutegfx/Platform.hpp>

#include "../Window.hpp"

class WindowManager {
public:
	using UpdateType = cutegfx::Platform::TickType;

	void registerVisibleWindow(const ctl::RcPtr<Window> &window);

	void unregisterVisibleWindow(const ctl::RcPtr<Window> &window);

	ctl::RcPtr<Window> getLastActiveWindow();

	void setLastActiveWindow(const ctl::RcPtr<Window> &window);

	void updateWindows();

	void startRenderThread();

	void joinRenderThread();

	void setWaitSync(bool waitSync = true);

	ctl::Signal<UpdateType> sUpdate;

private:
	std::unordered_set<ctl::RcPtr<Window>> _visibleWindows;
	ctl::RcPtr<Window> _lastActiveWindow;
	std::thread _renderThread;
	std::mutex _visibleWindowsMutex;
	std::mutex _lastActiveWindowMutex;
	std::atomic<bool> _visibleWindowsChanged = false;
	std::atomic<bool> _waitSync = true;

	void renderMain();
};