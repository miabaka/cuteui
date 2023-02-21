#pragma once

#include <memory>
#include "Device.hpp"

namespace cutegfx {

class Renderer {
public:
	explicit Renderer(std::shared_ptr<Device> device);

	std::shared_ptr<Viewport> createViewport();

	void render();

private:
	std::shared_ptr<Device> _device;
};

} // namespace cutegfx