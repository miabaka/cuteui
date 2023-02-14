#include "cute/ui/Window.hpp"

using namespace cute::ui;

Window::Window(int width, int height, const std::string &title) {

}

void Window::show() {

}

void Window::hide() {

}

void Window::setVisible(bool visible) {
	if (visible)
		show();
	else
		hide();
}