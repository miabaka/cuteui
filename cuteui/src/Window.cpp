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

bool Window::isVisible() const {
	return _platformWindow->isVisible();
}

void Window::setVisible(bool visible) {
	_platformWindow->setVisible(true);
}

Window::BackdropType Window::getBackdropType() const {
	return _platformWindow->getBackdropType();
}

bool Window::setBackdropType(BackdropType backdropType) {
	return _platformWindow->setBackdropType(backdropType);
}

void Window::setMainWidget(std::shared_ptr<Widget> widget) {
	_mainWidget = std::move(widget);
}

void Window::onVisibilityChange(bool visible) {
	auto &wm = Application::getInstance().getWindowManager();

	if (visible)
		wm.registerWindow(asShared());
	else
		wm.unregisterWindow(asShared());
}

void Window::render(bool waitSync) {
	_viewport->present(waitSync);
}