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

	void registerWindow(Window *window);

	void unregisterWindow(Window *window);

	void registerVisibleWindow(std::shared_ptr<Window> window);

	void unregisterVisibleWindow(std::shared_ptr<Window> window);

	void updateWindows();

	void startRenderThread();

	void joinRenderThread();

	cuteutil::SingleSignal<UpdateType> sUpdate;

private:
	std::unordered_set<Window *> _windows;
	std::unordered_set<std::shared_ptr<Window>> _visibleWindows;
	std::thread _renderThread;
	std::mutex _visibleWindowsMutex;
	std::atomic<bool> _visibleWindowsChanged = false;

	void renderMain();
};