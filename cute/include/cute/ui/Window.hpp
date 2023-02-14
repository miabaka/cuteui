#pragma once

#include <string>
#include "Object.hpp"

namespace cute::ui {

class Window : public Object {
public:
	Window(int width, int height, const std::string &title);

	void show();

	void hide();

	void setVisible(bool visible = true);
};

} // namespace cute::ui