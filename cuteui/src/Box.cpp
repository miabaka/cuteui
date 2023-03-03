#include "cuteui/Box.hpp"

Box::Box(Direction direction, int spacing)
		: _direction(direction),
		  _spacing(spacing) {}

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

	int spacing = _spacing * std::max(0, static_cast<int>(_children.size()) - 1);

	if (_direction == Direction::Horizontal)
		requiredSize.x += spacing;
	else
		requiredSize.y += spacing;

	return requiredSize;
}

void Box::updateLayout(glm::ivec2 position, glm::ivec2 availableSpace) {
	Widget::updateLayout(position, availableSpace);

	glm::ivec2 cursor = getPosition();
	glm::ivec2 requiredSize = computeRequiredSize();

	for (auto &child: _children) {
		bool isLastWidget = (child == _children.back());
		int spacing = (isLastWidget ? 0 : _spacing);

		glm::ivec2 size = child->computeRequiredSize();
		glm::ivec2 advance{};

		if (_direction == Direction::Horizontal) {
			advance.x = size.x + spacing;
			size.y = (getVerticalAlignment() == Alignment::Fill) ? availableSpace.y : requiredSize.y;
		} else {
			size.x = (getHorizontalAlignment() == Alignment::Fill) ? availableSpace.x : requiredSize.x;
			advance.y = size.y + spacing;
		}

		child->updateLayout(cursor, size);

		cursor += advance;
	}
}

void Box::draw(cutegfx::Renderer &renderer) {
	for (auto &child: _children)
		child->draw(renderer);
}

ctl::RcPtr<Widget> Box::getWidgetAtPoint(glm::ivec2 point) {
	if (!containsPoint(point))
		return nullptr;

	for (auto &child: _children) {
		ctl::RcPtr widgetAtPoint = child->getWidgetAtPoint(point);

		if (!widgetAtPoint)
			continue;

		return widgetAtPoint;
	}

	return this;
}

Direction Box::getDirection() const {
	return _direction;
}

void Box::setDirection(Direction direction) {
	_direction = direction;
}

int Box::getSpacing() const {
	return _spacing;
}

void Box::setSpacing(int spacing) {
	_spacing = spacing;
}

void Box::add(const ctl::RcPtr<Widget> &widget) {
	_children.emplace_back(widget);
}