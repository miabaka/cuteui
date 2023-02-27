#pragma once

#include <memory>
#include <stdexcept>
#include <utility>

namespace cuteutil {

template<typename T>
class SharedObject {
public:
	template<typename ...TArgs>
	static std::shared_ptr<T> create(const TArgs &...args) {
		auto object = std::make_shared<T>(args...);
		object->_weakThis = object;
		return object;
	}

	std::shared_ptr<T> asShared() const {
		if (_weakThis.expired())
			throw std::runtime_error("Derived from SharedObject objects must be instantiated with ::create()");

		return _weakThis.lock();
	}

private:
	std::weak_ptr<T> _weakThis;
};

} // namespace cuteutil