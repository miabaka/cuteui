#pragma once

#include <glm/glm.hpp>
#include "Widget.hpp"

class ColoredThingy : public Widget {
public:
	explicit ColoredThingy(glm::vec4 color);

	glm::ivec2 computeRequiredSize() override;

	void draw(cutegfx::Renderer &renderer) override;

private:
	glm::ivec2 _position{};
	glm::ivec2 _size{};
	glm::vec4 _color;
};