#include "cutegfx/Platform.hpp"

#include "Win32Platform.hpp"

using namespace cutegfx;

std::unique_ptr<Platform> Platform::createBest() {
	return std::make_unique<Win32Platform>();
}