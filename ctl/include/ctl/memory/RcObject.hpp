#pragma once

#include <atomic>

namespace ctl {

template<typename T>
class WeakRcPtr;

class RcObject {
	template<typename T>
	friend class WeakRcPtr;

public:
	RcObject() = default;

	RcObject(RcObject &) = delete;

	RcObject(RcObject &&) = delete;

	virtual ~RcObject() = default;

	inline void addRef() {
		_refCount++;
	}

	inline void release() {
		if (_refCount.fetch_sub(1, std::memory_order_acq_rel) > 1)
			return;

		_weakInfo->_objectAlive = false;
		_weakInfo->release();

		delete this;
	}

private:
	struct WeakInfo {
		std::atomic_bool _objectAlive = true;
		std::atomic_uint _refCount = 1;

		inline void addRef() {
			_refCount++;
		}

		inline void release() {
			if (_refCount.fetch_sub(1, std::memory_order_acq_rel) > 1)
				return;

			delete this;
		}
	};

	std::atomic_uint _refCount = 0;
	WeakInfo *_weakInfo = new WeakInfo();
};

} // namespace ctl