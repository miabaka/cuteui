#pragma once

#include <functional>
#include <list>
#include <utility>

namespace cuteutil {

template<typename ...TArgs>
class Signal {
public:
	Signal() = default;

	Signal(Signal &) = delete;

	Signal(Signal &&) = delete;

	template<typename TClass, typename TClassInstance>
	void bind(void (TClass::*method)(const TArgs &...), TClassInstance *instance) {
		_handler = [instance, method](const TArgs &...args) {
			std::invoke(method, instance, args...);
		};
	}

	void bind(std::function<void(const TArgs &...)> handler) {
		_handler = std::move(handler);
	}

	void reset() {
		_handler = {};
	}

	void emit(const TArgs &...args) {
		if (!_handler)
			return;

		_handler(args...);
	}

private:
	std::function<void(const TArgs &...)> _handler;
};

} // namespace cuteutil