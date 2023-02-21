#pragma once

#include <memory>
#include <string>

#include <glm/vec2.hpp>
#include <cutegfx/Viewport.hpp>
#include <cutegfx/PlatformWindow.hpp>
#include <cuteutil/SharedObject.hpp>

#include "Widget.hpp"

class WindowManager;

class Window : public cuteutil::SharedObject<Window>, public Widget {
	friend class WindowManager;

public:
	using BackdropType = cutegfx::PlatformWindow::BackdropType;
	using BorderType = cutegfx::PlatformWindow::BorderType;

	Window(glm::ivec2 size, const std::string &title);

	~Window() override;

	bool isVisible() const override;

	void setVisible(bool visible) override;

	BackdropType getBackdropType() const;

	bool setBackdropType(BackdropType backdropType);

	void setMainWidget(std::shared_ptr<Widget> widget);

private:
	std::shared_ptr<cutegfx::PlatformWindow> _platformWindow;
	std::shared_ptr<cutegfx::Viewport> _viewport;

	std::shared_ptr<Widget> _mainWidget;

	void onVisibilityChange(bool visible);

	void update();

	void render(bool waitSync = true);
};