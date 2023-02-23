#pragma once

#include <memory>

#include "Buffer.hpp"
#include "Viewport.hpp"

namespace cutegfx {

class Device {
public:
	virtual ~Device() = default;

	virtual std::shared_ptr<Buffer> createBuffer(Buffer::Type type) = 0;

	virtual std::shared_ptr<Viewport> createViewport() = 0;
};

} // namespace cutegfx