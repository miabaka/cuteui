#pragma once

#include <ctl/Signal.hpp>
#include "Widget.hpp"

class Button : public Widget {
public:
	glm::ivec2 computeRequiredSize() override;

	void draw(cutegfx::Renderer &renderer) override;

	void onMousePress(glm::ivec2 position) override;

	void onMouseRelease(glm::ivec2 position) override;

	ctl::Signal<> sClick;

private:
	bool _pressed = false;
};