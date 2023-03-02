#pragma once

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <ctl/memory.hpp>

#include "DeviceObject.hpp"
#include "Window.hpp"

namespace cutegfx {

class Viewport : public DeviceObject {
public:
	virtual void setOutputWindow(const ctl::RcPtr<Window> &window) = 0;

	virtual void clear(glm::vec4 color) = 0;

	virtual void resize(glm::uvec2 size) = 0;

	virtual void present(bool waitSync) = 0;

	virtual void use() = 0;
};

} // namespace cutegfx