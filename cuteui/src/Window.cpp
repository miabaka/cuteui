#include "cuteui/Application.hpp"

#include <ctl/memory.hpp>
#include "cuteui/Window.hpp"

Window::Window(glm::ivec2 size, const std::string &title) {
	auto &app = Application::getInstance();

	cutegfx::Platform &platform = app.getPlatform();
	cutegfx::Renderer &renderer = platform.getRenderer();

	_platformWindow = platform.createWindow();

	_platformWindow->sVisibilityChange.bind(&Window::onVisibilityChange, this);
	_platformWindow->sFocus.bind(&Window::onFocus, this);
	_platformWindow->sMousePress.bind(&Window::onMousePress, this);
	_platformWindow->sMouseRelease.bind(&Window::onMouseRelease, this);

	_platformWindow->sResizeBegin.bind([]() {
		Application::getInstance().getWindowManager().setWaitSync(false);
	});

	_platformWindow->sResizeEnd.bind([]() {
		Application::getInstance().getWindowManager().setWaitSync();
	});

	_platformWindow->setClientSize(size);
	_platformWindow->setTitle(title);

	_viewport = renderer.createViewport();

	_viewport->setOutputWindow(_platformWindow);
}

Window::~Window() {
	_platformWindow->sVisibilityChange.reset();
	_platformWindow->sFocus.reset();
	_platformWindow->sResizeBegin.reset();
	_platformWindow->sResizeEnd.reset();
	_platformWindow->sMousePress.reset();
	_platformWindow->sMouseRelease.reset();
}

bool Window::isVisible() const {
	return _platformWindow->isVisible();
}

void Window::setVisible(bool visible) {
	_platformWindow->setVisible(visible);
}

glm::ivec2 Window::computeRequiredSize() {
	if (!_mainWidget)
		return {0, 0};

	return _mainWidget->computeRequiredSize();
}

void Window::updateLayout(glm::ivec2 position, glm::ivec2 availableSpace) {
	_viewportSize = availableSpace;

	if (!_mainWidget)
		return;

	_mainWidget->updateLayout(position, availableSpace);
}

void Window::updateLayout() {
	updateLayout({0, 0}, _platformWindow->getClientSize());
}

void Window::draw(cutegfx::Renderer &renderer) {
	renderer.setViewport(_viewport);

	renderer.resize(_viewportSize);
	renderer.clear({1.f, 1.f, 1.f, 1.f});

	if (!_mainWidget)
		return;

	_mainWidget->draw(renderer);
}

ctl::RcPtr<Widget> Window::getWidgetAtPoint(glm::ivec2 point) {
	if (!_mainWidget)
		return nullptr;

	return _mainWidget->getWidgetAtPoint(point);
}

void Window::onMousePress(glm::ivec2 position) {
	ctl::RcPtr<Widget> widgetUnderCursor = getWidgetAtPoint(position);

	if (!widgetUnderCursor)
		return;

	widgetUnderCursor->onMousePress(position);

	_pressedWidget = widgetUnderCursor;
}

void Window::onMouseRelease(glm::ivec2 position) {
	ctl::RcPtr<Widget> pressedWidget;

	if (_pressedWidget) {
		_pressedWidget->onMouseRelease(position);
		std::swap(pressedWidget, _pressedWidget);
	}

	ctl::RcPtr<Widget> widgetUnderCursor = getWidgetAtPoint(position);

	if (!(widgetUnderCursor && widgetUnderCursor != pressedWidget))
		return;

	widgetUnderCursor->onMouseRelease(position);
}

Window::BackdropType Window::getBackdropType() const {
	return _platformWindow->getBackdropType();
}

bool Window::setBackdropType(BackdropType backdropType) {
	return _platformWindow->setBackdropType(backdropType);
}

void Window::setMainWidget(const ctl::RcPtr<Widget> &widget) {
	_mainWidget = widget;
}

void Window::onVisibilityChange(bool visible) {
	auto &wm = Application::getInstance().getWindowManager();

	if (visible)
		wm.registerVisibleWindow(this);
	else
		wm.unregisterVisibleWindow(this);
}

void Window::onFocus() {
	Application::getInstance().getWindowManager().setLastActiveWindow(this);
}

void Window::present(bool waitSync) {
	_viewport->present(waitSync);
}