#include "cuteui/Application.hpp"

#include <iostream>
#include "cuteui/Window.hpp"

Window::Window(glm::ivec2 size, const std::string &title) {
	auto &app = Application::getInstance();

	Platform &platform = app.getPlatform();
	Renderer &renderer = platform.getRenderer();

	_platformWindow = platform.createWindow();

	_platformWindow->setClientSize(size);
	_platformWindow->setTitle(title);

	_platformWindow->visibilityChanged.bind(&Window::onVisibilityChange, this);

	_viewport = renderer.createViewport();

	_viewport->setOutputWindow(_platformWindow);
}

void Window::show() {
	_platformWindow->setVisible(true);
}

void Window::setMainWidget(std::shared_ptr<Widget> widget) {
	_mainWidget = std::move(widget);
}

void Window::onVisibilityChange(bool visible) {
	auto &app = Application::getInstance();

	if (visible)
		app.registerVisibleWindow(this);
	else
		app.unregisterVisibleWindow(this);
}