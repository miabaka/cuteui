#pragma once

#include <cute/ui/Application.hpp>
#include <cute/ui/Window.hpp>

class NyaApplication : public cute::ui::Application {
public:
	NyaApplication();

private:
	cute::ui::Window _window;
};