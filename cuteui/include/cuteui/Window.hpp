#pragma once

#include <memory>
#include <string>

#include <glm/vec2.hpp>
#include <cutegfx/Viewport.hpp>
#include <cutegfx/PlatformWindow.hpp>

#include "Widget.hpp"

class Window : public Widget {
public:
	using BackdropType = PlatformWindow::BackdropType;
	using BorderType = PlatformWindow::BorderType;

	Window(glm::ivec2 size, const std::string &title);

	bool isVisible() const override;

	void setVisible(bool visible) override;

	BackdropType getBackdropType() const;

	bool setBackdropType(BackdropType backdropType);

	void setMainWidget(std::shared_ptr<Widget> widget);

private:
	std::shared_ptr<PlatformWindow> _platformWindow;
	std::shared_ptr<Viewport> _viewport;

	std::shared_ptr<Widget> _mainWidget;

	void onVisibilityChange(bool visible);
};