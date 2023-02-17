#pragma once

#include <string>

#include "Widget.hpp"

class Button : public Widget {
public:
	Button(const std::string &name);

	bool isVisible() const override;

	void setVisible(bool visible) override;
};