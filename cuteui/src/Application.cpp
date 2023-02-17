#include "cuteui/Application.hpp"

#include <stdexcept>
#include <cutegfx/CuteGfx.hpp>

Application *Application::_instance = nullptr;

Application &Application::getInstance() {
	if (!_instance)
		throw std::runtime_error("You must first create an Application");

	return *_instance;
}

Application::Application() {
	if (_instance)
		throw std::runtime_error("Multiple Application instances can not exist simultaneously");

	_platform = CuteGfx::createPlatform();
	_instance = this;
}

Application::~Application() {
	_instance = nullptr;
}

Platform &Application::getPlatform() {
	return *_platform;
}

int Application::run() {
	while (hasVisibleWindows())
		_platform->waitEvents();

	return 0;
}

bool Application::hasVisibleWindows() const {
	return !_visibleWindows.empty();
}

void Application::registerVisibleWindow(Window *window) {
	_visibleWindows.insert(window);
}

void Application::unregisterVisibleWindow(Window *window) {
	_visibleWindows.erase(window);
}