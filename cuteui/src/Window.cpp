#include "cuteui/Application.hpp"

#include <iostream>
#include "cuteui/Window.hpp"

Window::Window(glm::ivec2 size, const std::string &title) {
	auto &app = Application::getInstance();

	cutegfx::Platform &platform = app.getPlatform();
	cutegfx::Device &device = platform.getDevice();

	_platformWindow = platform.createWindow();

	_platformWindow->setClientSize(size);
	_platformWindow->setTitle(title);

	_platformWindow->visibilityChanged.bind(&Window::onVisibilityChange, this);

	_viewport = device.createViewport();

	_viewport->setOutputWindow(_platformWindow);

	app.getWindowManager().registerWindow(this);
}

Window::~Window() {
	Application::getInstance().getWindowManager().unregisterWindow(this);
	_platformWindow->visibilityChanged.reset();
}

bool Window::isVisible() const {
	return _platformWindow->isVisible();
}

void Window::setVisible(bool visible) {
	_platformWindow->setVisible(visible);
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
		wm.registerVisibleWindow(asShared());
	else
		wm.unregisterVisibleWindow(asShared());
}

void Window::update() {

}

void Window::render(bool waitSync) {
	_viewport->present(waitSync);
}