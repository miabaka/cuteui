#pragma once

#include <memory>

#include "PlatformWindow.hpp"
#include "Viewport.hpp"

class Renderer {
public:
	virtual ~Renderer() = default;

	virtual std::shared_ptr<Viewport> createViewport() = 0;
};