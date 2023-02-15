#pragma once

#include <string>

#include "Object.hpp"
#include "Widget.hpp"

class Button : public Object, public Widget {
public:
	Button(const std::string &name);
};