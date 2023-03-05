#include "cuteui/Button.hpp"

glm::ivec2 Button::computeRequiredSize() {
	return {72, 32};
}

void Button::draw(cutegfx::Renderer &renderer) {
	glm::vec4 color;

	if (_pressed)
		color = {0xff, 0xff, 0xff, 0xff};
	else
		color = {0xf0, 0xf0, 0xf0, 0xff};

	renderer.fillRect(getPosition(), getPosition() + getActualSize(), color);
}

void Button::onMousePress(glm::ivec2 position) {
	_pressed = true;
}

void Button::onMouseRelease(glm::ivec2 position) {
	if (!(_pressed && containsPoint(position))) {
		_pressed = false;
		return;
	}

	sClick();

	_pressed = false;
}