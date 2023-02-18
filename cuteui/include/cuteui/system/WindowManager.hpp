#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "../Window.hpp"

class WindowManager {
public:
	bool hasWindows() const;

	void registerWindow(std::shared_ptr<Window> window);

	void unregisterWindow(std::shared_ptr<Window> window);

	void tickWindows();

private:
	std::unordered_set<std::shared_ptr<Window>> _windows;
};