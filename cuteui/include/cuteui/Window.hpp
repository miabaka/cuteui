#pragma once

#include <memory>
#include <string>

#include <glm/vec2.hpp>
#include <ctl/memory.hpp>
#include <cutegfx/Viewport.hpp>
#include <cutegfx/Window.hpp>

#include "Widget.hpp"

class WindowManager;

class Window : public Widget {
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

	ctl::RcPtr<Widget> getWidgetAtPoint(glm::ivec2 point) override;

	void onMousePress(glm::ivec2 position) override;

	void onMouseRelease(glm::ivec2 position) override;

	BackdropType getBackdropType() const;

	bool setBackdropType(BackdropType backdropType);

	void setMainWidget(const ctl::RcPtr<Widget> &widget);

private:
	ctl::RcPtr<cutegfx::Window> _platformWindow;
	ctl::RcPtr<cutegfx::Viewport> _viewport;
	ctl::RcPtr<Widget> _mainWidget;
	glm::vec2 _viewportSize{};
	ctl::RcPtr<Widget> _pressedWidget;

	void onVisibilityChange(bool visible);

	void onFocus();

	void present(bool waitSync = true);
};