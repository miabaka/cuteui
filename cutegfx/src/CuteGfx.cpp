#include "cutegfx/CuteGfx.hpp"

#include "Win32Platform.hpp"

std::unique_ptr<Platform> CuteGfx::createPlatform() {
	return std::make_unique<Win32Platform>();
}