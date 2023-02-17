#pragma once

#include <memory>
#include <string>

#include "Renderer.hpp"
#include "PlatformWindow.hpp"

class Platform {
public:
	virtual ~Platform() = default;

	virtual std::shared_ptr<PlatformWindow> createWindow() = 0;

	virtual Renderer &getRenderer() = 0;

	virtual void waitEvents() = 0;
};