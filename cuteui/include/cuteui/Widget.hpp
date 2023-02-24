#pragma once

#include <glm/vec2.hpp>
#include <cutegfx/Renderer.hpp>

#include "Alignment.hpp"

class Widget {
public:

	virtual ~Widget() = default;

	virtual bool isVisible() const;

	virtual void setVisible(bool visible);

	virtual Alignment getHorizontalAlignment() const;

	virtual void setHorizontalAlignment(Alignment alignment);

	virtual Alignment getVerticalAlignment() const;

	virtual void setVerticalAlignment(Alignment alignment);

	virtual void updateLayout(glm::ivec2 position, glm::ivec2 maxSize) = 0;

	virtual void draw(cutegfx::Renderer &renderer) = 0;

private:
	bool _visible = true;
	Alignment _horizontalAlignment = Alignment::Default;
	Alignment _verticalAlignment = Alignment::Default;
};