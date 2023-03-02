#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Renderer.hpp"
#include "Window.hpp"

namespace cutegfx {

class Platform {
public:
	enum class TickType {
		Update,
		EventLoopStopRequest
	};

	static std::unique_ptr<Platform> createBest();

	virtual ~Platform() = default;

	virtual ctl::RcPtr<Window> createWindow() = 0;

	virtual Renderer &getRenderer() = 0;

	virtual int runEventLoop(std::function<void(void)> tickHandler) = 0;

	virtual void executeTickHandlerIndirect(TickType tickType) = 0;
};

} // namespace cutegfx