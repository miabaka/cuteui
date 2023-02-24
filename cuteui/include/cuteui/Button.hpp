#pragma once

#include <string>
#include "Widget.hpp"

class Button : public Widget {
public:
	explicit Button(const std::string &name);

	void updateLayout(glm::ivec2 position, glm::ivec2 maxSize) override;

	void draw(cutegfx::Renderer &renderer) override;

private:
	glm::ivec2 _position{};
	glm::ivec2 _size{};
};