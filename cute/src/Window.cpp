#include "cute/ui/Application.hpp"
#include "cute/ui/Window.hpp"

Window::Window(int width, int height, const std::string &title)
		: _platformWindow(Application::getInstance().getPlatform().createWindow()) {
	_platformWindow->setClientSize(width, height);
	_platformWindow->setTitle(title);
}

void Window::show() {
	_platformWindow->show();
}

void Window::setMainWidget(std::shared_ptr<Widget> widget) {
	_mainWidget = std::move(widget);
}