#pragma once

#include <functional>
#include <list>

namespace cuteutil {

template<typename ...TArgs>
class Signal {
public:
	Signal() = default;

	Signal(Signal &) = delete;

	Signal(Signal &&) = delete;

	template<typename TClass, typename TClassInstance>
	void bind(void (TClass::*method)(const TArgs &...), TClassInstance *instance) {
		_handlers.emplace_back([instance, method](const TArgs &...args) {
			std::invoke(method, instance, args...);
		});
	}

	template<typename TClass, typename TClassInstance>
	void bind(void (TClass::*method)(TArgs ...), TClassInstance *instance) {
		_handlers.emplace_back([instance, method](const TArgs &...args) {
			std::invoke(method, instance, args...);
		});
	}

	void emit(const TArgs &...args) {
		for (auto &handler: _handlers)
			handler(args...);
	}

private:
	std::list<std::function<void(const TArgs &...)>> _handlers;
};

} // namespace cuteutil