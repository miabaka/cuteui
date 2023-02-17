#pragma once

#include <memory>
#include "PlatformWindow.hpp"

class Viewport {
public:
	virtual ~Viewport() = default;

	virtual void setOutputWindow(std::shared_ptr<PlatformWindow> window) = 0;

	virtual void present(bool waitSync) = 0;
};