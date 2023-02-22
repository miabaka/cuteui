#include "cutegfx/Platform.hpp"

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

using namespace cutegfx;

std::unique_ptr<Platform> Platform::createBest() {
#ifdef _WIN32
	return std::make_unique<Win32Platform>();
#else
	return nullptr;
#endif
}