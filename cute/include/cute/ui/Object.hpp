#pragma once

namespace cute::ui {

class Object {
public:
	Object() = default;

	Object(Object &) = delete;

	Object(Object &&) = delete;

	~Object() = default;
};

} // namespace cute::ui