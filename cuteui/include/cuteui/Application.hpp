#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

#include <cutegfx/Platform.hpp>

#include "system/WindowManager.hpp"

class Window;

class Application {
public:
	static Application &getInstance();

	Application();

	Application(Application &) = delete;

	Application(Application &&) = delete;

	~Application();

	Platform &getPlatform();

	int run();

	WindowManager &getWindowManager();

private:
	static Application *_instance;
	std::unique_ptr<Platform> _platform;
	WindowManager _windowManager;
};