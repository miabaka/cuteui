#pragma once

#include <memory>

#include "Widget.hpp"

enum class Direction {
	Horizontal,
	Vertical
};

class Box : public Widget {
public:
	explicit Box(Direction direction = Direction::Horizontal);

	bool isVisible() const override;

	void setVisible(bool visible) override;

	void add(std::shared_ptr<Widget> widget);
};