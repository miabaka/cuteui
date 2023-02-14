#pragma once

#include "Object.hpp"

namespace cute::ui {

class Application : public Object {
public:
	int run();
};

} // namespace cute::ui