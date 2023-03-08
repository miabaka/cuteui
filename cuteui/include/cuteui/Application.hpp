#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_set>

#include <cutegfx/Platform.hpp>

#include "system/Theme.hpp"
#include "system/WindowManager.hpp"

class Window;

class Application {
public:
	static Application &getInstance();

	Application();

	Application(Application &) = delete;

	Application(Application &&) = delete;

	~Application();

	int run();

	cutegfx::Platform &getPlatform();

	WindowManager &getWindowManager();

	Theme &getTheme();

private:
	static Application *_instance;
	std::unique_ptr<cutegfx::Platform> _platform;
	WindowManager _windowManager;
	Theme _theme{};

	void prepareTheme();
};