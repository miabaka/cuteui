#pragma once

#include <memory>
#include <unordered_set>

#include <cutegfx/Platform.hpp>

class Window;

class Application {
	friend class Window;

public:
	static Application &getInstance();

	Application();

	Application(Application &) = delete;

	Application(Application &&) = delete;

	~Application();

	Platform &getPlatform();

	int run();

	bool hasVisibleWindows() const;

private:
	static Application *_instance;
	std::unique_ptr<Platform> _platform;
	std::unordered_set<Window *> _visibleWindows;

	void registerVisibleWindow(Window *window);

	void unregisterVisibleWindow(Window *window);
};