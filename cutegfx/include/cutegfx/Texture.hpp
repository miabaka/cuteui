#pragma once

#include <ctl/memory.hpp>

namespace cutegfx {

class Texture : public ctl::RcObject {
public:
	virtual void use() = 0;
};

} // namespace cutegfx