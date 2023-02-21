#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Device.hpp"
#include "PlatformWindow.hpp"

namespace cutegfx {

class Platform {
public:
	enum class TickType {
		Update,
		EventLoopStopRequest
	};

	static std::unique_ptr<Platform> createBest();

	virtual ~Platform() = default;

	virtual std::shared_ptr<PlatformWindow> createWindow() = 0;

	virtual Device &getDevice() = 0;

	virtual void runEventLoop(std::function<void(void)> tickHandler) = 0;

	virtual void executeTickHandlerIndirect(TickType tickType) = 0;
};

} // namespace cutegfx