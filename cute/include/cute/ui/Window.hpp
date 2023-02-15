#pragma once

#include <memory>
#include <string>

#include "platform/PlatformWindow.hpp"
#include "Object.hpp"
#include "Widget.hpp"

class Window : public Object {
public:
	Window(int width, int height, const std::string &title);

	void show();

	void setMainWidget(std::shared_ptr<Widget> widget);

private:
	std::unique_ptr<PlatformWindow> _platformWindow;

	std::shared_ptr<Widget> _mainWidget;
};