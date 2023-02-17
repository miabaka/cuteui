#pragma once

class Widget {
public:
	virtual ~Widget() = default;

	virtual bool isVisible() const = 0;

	virtual void setVisible(bool visible) = 0;
};