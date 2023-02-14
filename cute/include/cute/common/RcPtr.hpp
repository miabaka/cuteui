#include <memory>

namespace cute::common {

template<typename T>
class RcPtr : public std::shared_ptr<T> {
public:
	explicit RcPtr(std::shared_ptr<T> &&ptr) : std::shared_ptr<T>(ptr) {}

	template<typename ...Targs>
	inline static auto from(Targs &&...args) {
		return RcPtr(std::make_shared<T>(args...));
	}
};

} // namespace cute::commons