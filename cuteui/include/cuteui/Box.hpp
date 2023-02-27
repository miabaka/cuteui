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
	explicit Box(Direction direction = Direction::Horizontal);

	glm::ivec2 computeRequiredSize() override;

	void updateLayout(glm::ivec2 position, glm::ivec2 availableSpace) override;

	void draw(cutegfx::Renderer &renderer) override;

	std::shared_ptr<Widget>
	getWidgetAtPoint(glm::ivec2 point, const std::shared_ptr<Widget> &defaultWidget) const override;

	Direction getDirection() const;

	void setDirection(Direction direction);

	void add(const std::shared_ptr<Widget> &widget);

private:
	std::vector<std::shared_ptr<Widget>> _children;
	Direction _direction;
};