#include "cuteui/Application.hpp"

#include <stdexcept>

Application *Application::_instance = nullptr;

Application &Application::getInstance() {
	if (!_instance)
		throw std::runtime_error("You must first create an Application");

	return *_instance;
}

Application::Application() {
	if (_instance)
		throw std::runtime_error("Multiple Application instances can not exist simultaneously");

	_platform = cutegfx::Platform::createBest();
	_instance = this;
}

Application::~Application() {
	_instance = nullptr;
}

cutegfx::Platform &Application::getPlatform() {
	return *_platform;
}

int Application::run() {
	_windowManager.sUpdate.bind([this](WindowManager::UpdateType type) {
		_platform->executeTickHandlerIndirect(type);
	});

	_windowManager.startRenderThread();

	_platform->runEventLoop([this]() {
		_windowManager.updateWindows();
	});

	_windowManager.joinRenderThread();

	return 0;
}

WindowManager &Application::getWindowManager() {
	return _windowManager;
}