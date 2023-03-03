#pragma once

#include "RcObject.hpp"
#include "RcPtr.hpp"

namespace ctl {

// TODO: make it actually atomic
template<typename T>
class WeakRcPtr {
public:
	WeakRcPtr() = default;

	WeakRcPtr(T *object) : _object(object) {
		if (!_object)
			return;

		_weakInfo = object->_weakInfo;
		_weakInfo->addRef();
	}

	WeakRcPtr(const RcPtr<T> &other) : WeakRcPtr(other.get()) {};

	WeakRcPtr(const WeakRcPtr &other)
			: _object{other._object},
			  _weakInfo(other._weakInfo) {
		if (_weakInfo)
			_weakInfo->addRef();
	};

	WeakRcPtr(WeakRcPtr &&other) noexcept
			: _object(other._object),
			  _weakInfo(other._weakInfo) {
		other._object = nullptr;
		other._weakInfo = nullptr;
	}

	~WeakRcPtr() {
		if (_weakInfo)
			_weakInfo->release();
	}

	WeakRcPtr &operator=(const RcPtr<T> &ptr) {
		if (_weakInfo)
			_weakInfo->release();

		_object = ptr._object;

		if (!_object) {
			_weakInfo = nullptr;
		} else {
			_weakInfo = _object->_weakInfo;
			_weakInfo->addRef();
		}

		return *this;
	}

	operator bool() const {
		if (!_weakInfo)
			return false;

		return _weakInfo->_objectAlive;
	}

	RcPtr<T> lock() const {
		if (!_weakInfo->_objectAlive)
			return nullptr;

		return _object;
	}

private:
	T *_object = nullptr;
	RcObject::WeakInfo *_weakInfo = nullptr;
};

} // namespace ctl