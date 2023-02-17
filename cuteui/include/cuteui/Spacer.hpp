#pragma once

#include "Widget.hpp"

class Spacer : public Widget {
public:
	Spacer(int weight = 1);

	bool isVisible() const override;

	void setVisible(bool visible) override;
};