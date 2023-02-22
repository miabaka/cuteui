#include "cuteui/Application.hpp"

#include "cuteui/Window.hpp"

Window::Window(glm::ivec2 size, const std::string &title) {
	auto &app = Application::getInstance();

	cutegfx::Platform &platform = app.getPlatform();
	cutegfx::Renderer &renderer = platform.getRenderer();

	_platformWindow = platform.createWindow();

	_platformWindow->sVisibilityChange.bind(&Window::onVisibilityChange, this);
	_platformWindow->sFocus.bind(&Window::onFocus, this);

	_platformWindow->setClientSize(size);
	_platformWindow->setTitle(title);

	_viewport = renderer.createViewport();

	_viewport->setOutputWindow(_platformWindow);

	app.getWindowManager().registerWindow(this);
}

Window::~Window() {
	Application::getInstance().getWindowManager().unregisterWindow(this);
	_platformWindow->sVisibilityChange.reset();
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

void Window::onVisibilityChange(const bool &visible) {
	auto &wm = Application::getInstance().getWindowManager();

	if (visible)
		wm.registerVisibleWindow(asShared());
	else
		wm.unregisterVisibleWindow(asShared());
}

void Window::onFocus() {
	Application::getInstance().getWindowManager().setLastActiveWindow(asShared());
}

void Window::updateAndDraw() {
	cutegfx::Renderer &renderer = Application::getInstance()
			.getPlatform()
			.getRenderer();

	renderer.setViewport(_viewport);

	renderer.resize(_platformWindow->getClientSize());
	renderer.clear({0.75f, 0.5f, 1.f, 0.75f});
}

void Window::present(bool waitSync) {
	_viewport->present(waitSync);
}