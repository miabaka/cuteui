#pragma once

#include <memory>
#include "Window.hpp"

namespace cutegfx {

class Viewport {
public:
	virtual ~Viewport() = default;

	virtual void setOutputWindow(std::shared_ptr<Window> window) = 0;

	virtual void present(bool waitSync) = 0;
};

} // namespace cutegfx