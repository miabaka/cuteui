#include "cute/ui/Application.hpp"
#include "platform/Win32Platform.hpp"

Application *Application::_instance = nullptr;

Application &Application::getInstance() {
	if (!_instance)
		throw std::runtime_error("You must first create an Application");

	return *_instance;
}

Application::Application() {
	if (_instance)
		throw std::runtime_error("Multiple Application instances can not exist simultaneously");

	_platform = std::make_unique<Win32Platform>();
	_instance = this;
}

Application::~Application() {
	_instance = nullptr;
}

Platform &Application::getPlatform() {
	return *_platform;
}

int Application::run() {
	return 0;
}