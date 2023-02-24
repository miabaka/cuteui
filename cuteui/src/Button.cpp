#include "cuteui/Button.hpp"

Button::Button(const std::string &name) {}

void Button::updateLayout(glm::ivec2 position, glm::ivec2 maxSize) {
	switch (getVerticalAlignment()) {
		case Alignment::Default:
		case Alignment::Fill:
			_position.x = position.x;
			_size.x = maxSize.x;

		case Alignment::Center:
			_position.x = (maxSize.x - 72) / 2 + position.x;
			_size.x = 72;
			break;

		case Alignment::Start:
		case Alignment::End:
			break;
	}

	switch (getHorizontalAlignment()) {
		case Alignment::Default:
		case Alignment::Fill:
			_position.y = position.y;
			_size.y = maxSize.y;

		case Alignment::Center:
			_position.y = (maxSize.y - 32) / 2 + position.y;
			_size.y = 32;
			break;

		case Alignment::Start:
		case Alignment::End:
			break;
	}
}

void Button::draw(cutegfx::Renderer &renderer) {
	renderer.fillRect(_position, _position + _size, {0.9f, 0.9f, 0.9f, 1.f});
}