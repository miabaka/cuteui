#include "cuteui/Widget.hpp"

bool Widget::isVisible() const {
	return _visible;
}

void Widget::setVisible(bool visible) {
	_visible = visible;
}

Alignment Widget::getHorizontalAlignment() const {
	return _horizontalAlignment;
}

void Widget::setHorizontalAlignment(Alignment alignment) {
	_horizontalAlignment = alignment;
}

Alignment Widget::getVerticalAlignment() const {
	return _verticalAlignment;
}

void Widget::setVerticalAlignment(Alignment alignment) {
	_verticalAlignment = alignment;
}

glm::ivec2 Widget::getActualSize() const {
	return _size;
}

glm::ivec2 Widget::getPosition() const {
	return _position;
}

template<typename T>
struct AlignAxisOut {
	T newPosition;
	T newActualSize;
};

template<int TAxis, typename T = glm::ivec2, typename Te=int>
static AlignAxisOut<Te> alignAxis(Alignment alignment, T &position_, T &available_, T &required_) {
	auto position = position_[TAxis];
	auto availableSpace = available_[TAxis];
	auto requiredSize = required_[TAxis];

	AlignAxisOut<Te> out{};

	out.newPosition = position;
	out.newActualSize = (alignment == Alignment::Fill) ? availableSpace : requiredSize;

	if (alignment == Alignment::Center)
		out.newPosition += (availableSpace - requiredSize) / 2;
	else if (alignment == Alignment::End)
		out.newPosition += availableSpace - requiredSize;

	return out;
}

void Widget::updateLayout(glm::ivec2 position, glm::ivec2 availableSpace) {
	glm::ivec2 requiredSize = computeRequiredSize();

	auto x = alignAxis<0>(getHorizontalAlignment(), position, availableSpace, requiredSize);
	auto y = alignAxis<1>(getVerticalAlignment(), position, availableSpace, requiredSize);

	setPosition({x.newPosition, y.newPosition});
	setActualSize({x.newActualSize, y.newActualSize});
}

bool Widget::containsPoint(glm::ivec2 point) const {
	glm::ivec2 p1 = getPosition();
	glm::ivec2 p2 = p1 + getActualSize();
	return point.x >= p1.x && point.x < p2.x && point.y >= p1.y && point.y < p2.y;
}

ctl::RcPtr<Widget> Widget::getWidgetAtPoint(glm::ivec2 point) {
	if (!containsPoint(point))
		return nullptr;

	return this;
}

void Widget::onMousePress(glm::ivec2 position) {}

void Widget::onMouseRelease(glm::ivec2 position) {}

void Widget::setActualSize(glm::ivec2 size) {
	_size = size;
}

void Widget::setPosition(glm::ivec2 position) {
	_position = position;
}