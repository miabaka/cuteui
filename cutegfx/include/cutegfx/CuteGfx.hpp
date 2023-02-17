#pragma once

#include <memory>
#include "Platform.hpp"

class CuteGfx {
public:
	static std::unique_ptr<Platform> createPlatform();

	CuteGfx() = delete;
};