#pragma once

#include <functional>
#include <list>

namespace cuteutil {

template<typename ...TArgs>
class SingleSignal {
public:
	SingleSignal() = default;

	SingleSignal(SingleSignal &) = delete;

	SingleSignal(SingleSignal &&) = delete;

	template<typename TClass, typename TClassInstance>
	void bind(void (TClass::*method)(const TArgs &...), TClassInstance *instance) {
		_handler = [instance, method](const TArgs &...args) {
			std::invoke(method, instance, args...);
		};
	}

	template<typename TClass, typename TClassInstance>
	void bind(void (TClass::*method)(TArgs ...), TClassInstance *instance) {
		_handler = [instance, method](const TArgs &...args) {
			std::invoke(method, instance, args...);
		};
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