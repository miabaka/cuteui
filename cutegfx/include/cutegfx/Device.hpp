#pragma once

#include <memory>

#include "Window.hpp"
#include "Viewport.hpp"

namespace cutegfx {

class Device {
public:
	virtual ~Device() = default;

	virtual std::shared_ptr<Viewport> createViewport() = 0;
};

} // namespace cutegfx