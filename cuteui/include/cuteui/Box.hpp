#pragma once

#include <memory>

#include "Object.hpp"
#include "Widget.hpp"

enum class Direction {
	Horizontal,
	Vertical
};

class Box : public Object, public Widget {
public:
	explicit Box(Direction direction = Direction::Horizontal);

	void add(std::shared_ptr<Widget> widget);
};