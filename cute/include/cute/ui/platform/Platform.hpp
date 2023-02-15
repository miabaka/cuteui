#pragma once

#include <memory>
#include <string>

#include "PlatformWindow.hpp"

class Platform {
public:
	virtual ~Platform() = default;

	virtual std::unique_ptr<PlatformWindow> createWindow() = 0;
};