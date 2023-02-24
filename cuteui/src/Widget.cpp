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