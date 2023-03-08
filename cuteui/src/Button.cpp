#include "cuteui/Button.hpp"

#include "cuteui/Application.hpp"

glm::ivec2 Button::computeRequiredSize() {
	return {72, 32};
}

void Button::draw(cutegfx::Renderer &renderer) {
	Theme &theme = Application::getInstance().getTheme();

	Theme::NinePatchImage &image = theme.images.buttonNormal;

	renderer.setTexture(image.texture);
	renderer.drawNinePatch(getPosition(), getPosition() + getActualSize(), image.metrics);
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