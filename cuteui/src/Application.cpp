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
	_windowManager.setUpdateHandler([this]() {
		_platform->executeTickHandlerIndirect();
	});

	_windowManager.setLastVisibleWindowClosedHandler([this]() {
		_platform->stopEventLoop();
	});

	_windowManager.startRenderThread();

	_platform->runEventLoop([this]() {
		_windowManager.updateWindows();
	});

	_windowManager.stopRenderThread();

	return 0;
}

WindowManager &Application::getWindowManager() {
	return _windowManager;
}