#pragma once

#include <glm/vec2.hpp>
#include <cutegfx/Renderer.hpp>

enum class Alignment {
	Fill,
	Center,
	Start,
	End
};

class Widget {
public:
	virtual ~Widget() = default;

	virtual bool isVisible() const;

	virtual void setVisible(bool visible);

	virtual Alignment getHorizontalAlignment() const;

	virtual void setHorizontalAlignment(Alignment alignment);

	virtual Alignment getVerticalAlignment() const;

	virtual void setVerticalAlignment(Alignment alignment);

	virtual glm::ivec2 getActualSize() const;

	virtual glm::ivec2 getPosition() const;

	virtual glm::ivec2 computeRequiredSize() = 0;

	virtual void updateLayout(glm::ivec2 position, glm::ivec2 availableSpace);

	virtual void draw(cutegfx::Renderer &renderer) = 0;

protected:
	virtual void setActualSize(glm::ivec2 size);

	virtual void setPosition(glm::ivec2 position);

private:
	bool _visible = true;
	Alignment _horizontalAlignment = Alignment::Fill;
	Alignment _verticalAlignment = Alignment::Fill;
	glm::ivec2 _size;
	glm::ivec2 _position;
};