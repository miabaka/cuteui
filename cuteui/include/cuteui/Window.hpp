#pragma once

#include <memory>
#include <string>

#include <glm/vec2.hpp>
#include <cutegfx/Viewport.hpp>
#include <cutegfx/PlatformWindow.hpp>

#include "Object.hpp"
#include "Widget.hpp"

class Window : public Object {
public:
	Window(glm::ivec2 size, const std::string &title);

	void show();

	void setMainWidget(std::shared_ptr<Widget> widget);

private:
	std::shared_ptr<PlatformWindow> _platformWindow;
	std::shared_ptr<Viewport> _viewport;

	std::shared_ptr<Widget> _mainWidget;

	void onVisibilityChange(bool visible);
};