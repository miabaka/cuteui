#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace ctl {

class RcObject;

template<typename T>
class WeakRcPtr;

// TODO: make it actually atomic
template<typename T>
class RcPtr {
	friend class WeakRcPtr<T>;

	template <typename U>
	friend class RcPtr;

public:
	RcPtr() = default;

	RcPtr(T *object) : _object(object) {
		if (!_object)
			return;

		object->addRef();
	};

	template<typename TOther>
	RcPtr(const RcPtr<TOther> other) : RcPtr(static_cast<T *>(other._object)) {}

	RcPtr(const RcPtr &other) : RcPtr(other._object) {}

	RcPtr(RcPtr<T> &&other) noexcept {
		_object = other._object;
		other._object = nullptr;
	}

	RcPtr<T> &operator=(T *object) {
		if (_object)
			_object->release();

		_object = object;

		if (_object)
			_object->addRef();

		return *this;
	}

	RcPtr<T> &operator=(const RcPtr<T> &other) {
		if (this != &other)
			operator=(other.get());

		return *this;
	}

	~RcPtr() {
		if (!_object)
			return;

		_object->release();
	}

	template<typename ...TArgs>
	static RcPtr alloc(TArgs &&...args) {
		return RcPtr<T>(new T(std::forward<TArgs>(args)...));
	}

	T *get() const {
		return _object;
	}

	T *operator->() const {
		return _object;
	};

	T &operator*() const {
		return *_object;
	}

	bool operator==(const RcPtr<T> &other) const {
		return other._object == _object;
	}

	bool operator!=(const RcPtr<T> &other) const {
		return !operator==(other);
	}

	explicit operator bool() const {
		return static_cast<bool>(_object);
	}

	template<typename TTargetType>
	RcPtr<TTargetType> dynamicCast() const {
		return dynamic_cast<TTargetType *>(_object);
	}

private:
	T *_object = nullptr;
};

} // namespace ctl

namespace std {

template<typename T>
struct hash<ctl::RcPtr<T>> {
	size_t operator()(const ctl::RcPtr<T> &ptr) const {
		return hash<void *>()(ptr.get());
	}
};

}  // namespace std