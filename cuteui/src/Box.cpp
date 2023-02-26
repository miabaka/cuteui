#include "cuteui/Box.hpp"

Box::Box(Direction direction)
		: _direction(direction) {}

glm::ivec2 Box::computeRequiredSize() {
	glm::ivec2 requiredSize{};

	for (auto &child: _children) {
		glm::ivec2 childRequiredSize = child->computeRequiredSize();

		if (_direction == Direction::Horizontal) {
			requiredSize.x += childRequiredSize.x;
			requiredSize.y = std::max(requiredSize.y, childRequiredSize.y);
		} else {
			requiredSize.x = std::max(requiredSize.x, childRequiredSize.x);
			requiredSize.y += childRequiredSize.y;
		}
	}

	return requiredSize;
}

void Box::updateLayout(glm::ivec2 position, glm::ivec2 availableSpace) {
	Widget::updateLayout(position, availableSpace);

	glm::ivec2 cursor = getPosition();

	for (auto &child: _children) {
		glm::ivec2 size = child->computeRequiredSize();

		child->updateLayout(cursor, size);

		if (_direction == Direction::Horizontal)
			cursor.x += size.x;
		else
			cursor.y += size.y;
	}
}

void Box::draw(cutegfx::Renderer &renderer) {
	for (auto &child: _children)
		child->draw(renderer);
}

void Box::add(const std::shared_ptr<Widget> &widget) {
	_children.emplace_back(widget);
}