#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "../Window.hpp"

class WindowManager {
public:
	bool hasVisibleWindows() const;

	void registerWindow(Window *window);

	void unregisterWindow(Window *window);

	void registerVisibleWindow(std::shared_ptr<Window> window);

	void unregisterVisibleWindow(std::shared_ptr<Window> window);

	void updateWindows();

	void renderWindows();

	void setUpdateHandler(std::function<void()> handler) {
		_updateHandler = std::move(handler);
	}

	void setLastVisibleWindowClosedHandler(std::function<void()> handler) {
		_lastVisibleWindowClosedHandler = std::move(handler);
	}

	void startRenderThread();

	void stopRenderThread();

private:
	std::unordered_set<Window *> _windows;
	std::unordered_set<std::shared_ptr<Window>> _visibleWindows;

	std::function<void()> _updateHandler;
	std::function<void()> _lastVisibleWindowClosedHandler;

	std::atomic<bool> _renderThreadRunning;
	std::thread _renderThread;

	void renderMain();
};