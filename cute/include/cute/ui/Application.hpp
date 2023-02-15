#pragma once

#include <memory>

#include "platform/Platform.hpp"
#include "Object.hpp"

class Application : public Object {
public:
	static Application &getInstance();

	static int execute();

	Application();

	~Application() override;

	Platform &getPlatform();

	int run();

private:
	static Application *_instance;
	std::unique_ptr<Platform> _platform;
};