#pragma once

class Object {
public:
	Object() = default;

	Object(Object &) = delete;

	Object(Object &&) = delete;

	virtual ~Object() = default;
};