#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "../Window.hpp"

class WindowManager {
public:
	~WindowManager();

	bool hasWindows() const;

	void registerWindow(std::shared_ptr<Window> window);

	void unregisterWindow(std::shared_ptr<Window> window);

private:
	std::unordered_set<std::shared_ptr<Window>> _windows;
	std::atomic<bool> _windowListChanged = false;
	std::atomic<bool> _hasWindows = false;
	std::mutex _windowListMutex;

	std::thread _renderThread;
	std::atomic<bool> _renderThreadIsRunning;

	void startRenderThread();

	void stopRenderThread();

	void renderMain();
};