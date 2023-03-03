#pragma once

#include <memory>
#include <vector>

#include "Widget.hpp"

enum class Direction {
	Horizontal,
	Vertical
};

class Box : public Widget {
public:
	explicit Box(Direction direction = Direction::Horizontal, int spacing = 0);

	glm::ivec2 computeRequiredSize() override;

	void updateLayout(glm::ivec2 position, glm::ivec2 availableSpace) override;

	void draw(cutegfx::Renderer &renderer) override;

	ctl::RcPtr<Widget> getWidgetAtPoint(glm::ivec2 point) override;

	Direction getDirection() const;

	void setDirection(Direction direction);

	int getSpacing() const;

	void setSpacing(int spacing);

	void add(const ctl::RcPtr<Widget> &widget);

private:
	std::vector<ctl::RcPtr<Widget>> _children;
	Direction _direction;
	int _spacing;
};