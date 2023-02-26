#include "cuteui/ColoredThingy.hpp"

ColoredThingy::ColoredThingy(glm::vec4 color)
		: _color(color) {}

glm::ivec2 ColoredThingy::computeRequiredSize() {
	return {64, 64};
}

void ColoredThingy::draw(cutegfx::Renderer &renderer) {
	renderer.fillRect(getPosition(), getPosition() + getActualSize(), _color);
}