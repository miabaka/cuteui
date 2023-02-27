#pragma once

#include <memory>
#include <string>

#include <glm/vec2.hpp>
#include <cutegfx/Viewport.hpp>
#include <cutegfx/Window.hpp>
#include <cuteutil/SharedObject.hpp>

#include "Widget.hpp"

class WindowManager;

class Window : public cuteutil::SharedObject<Window>, public Widget {
	friend class WindowManager;

public:
	using BackdropType = cutegfx::Window::BackdropType;
	using DecorationMode = cutegfx::Window::DecorationMode;

	Window(glm::ivec2 size, const std::string &title);

	~Window() override;

	bool isVisible() const override;

	void setVisible(bool visible) override;

	glm::ivec2 computeRequiredSize() override;

	void updateLayout(glm::ivec2 position, glm::ivec2 availableSpace) override;

	void updateLayout();

	void draw(cutegfx::Renderer &renderer) override;

	std::shared_ptr<Widget>
	getWidgetAtPoint(glm::ivec2 point, const std::shared_ptr<Widget> &defaultWidget) const override;

	void onMousePress(glm::ivec2 position) override;

	void onMouseRelease(glm::ivec2 position) override;

	BackdropType getBackdropType() const;

	bool setBackdropType(BackdropType backdropType);

	void setMainWidget(std::shared_ptr<Widget> widget);

private:
	std::shared_ptr<cutegfx::Window> _platformWindow;
	std::shared_ptr<cutegfx::Viewport> _viewport;
	std::shared_ptr<Widget> _mainWidget;
	glm::vec2 _viewportSize{};
	std::shared_ptr<Widget> _pressedWidget;

	void onVisibilityChange(bool visible);

	void onFocus();

	void present(bool waitSync = true);
};