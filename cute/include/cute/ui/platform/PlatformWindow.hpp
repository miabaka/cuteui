#pragma once

#include <string>

class PlatformWindow {
public:
	virtual ~PlatformWindow() = default;

	virtual void setClientSize(int width, int height) = 0;

	virtual void setTitle(const std::string &title) = 0;

	virtual void show() = 0;
};